/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Sami Rantanen <sami.rantanen@magister.fi>
 */

#ifndef SAT_FWD_LINK_SCHEDULER_H
#define SAT_FWD_LINK_SCHEDULER_H

#include <cstring>

#include "ns3/address.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/mac48-address.h"
#include "ns3/random-variable-stream.h"
#include "ns3/timer.h"

#include "satellite-mac.h"
#include "satellite-net-device.h"
#include "satellite-signal-parameters.h"
#include "satellite-scheduling-object.h"
#include "satellite-phy.h"
#include "satellite-bbframe.h"
#include "satellite-bbframe-container.h"
#include "satellite-cno-estimator.h"
#include "ns3/satellite-bbframe-conf.h"

namespace ns3 {

/**
 * \ingroup satellite
  * \brief GW specific Mac class for Sat Net Devices.
 *
 * This SatGwMac class specializes the Mac class with GW characteristics.
 */

class SatFwdLinkScheduler : public Object
{
public:
  /**
   * Types for sorting algorithm used by forward link scheduler
   */
  typedef enum
  {
    NO_SORT,             //!< NO_SORT
    BUFFERING_DELAY_SORT,//!< BUFFERING_DELAY_SORT
    BUFFERING_LOAD_SORT, //!< BUFFERING_LOAD_SORT
    RANDOM_SORT,         //!< RANDOM_SORT
    PRIORITY_SORT        //!< PRIORITY_SORT
  } ScheduleSortingCriteria_t;

  /**
   * BBFrame usage modes.
   */
  typedef enum
  {
    SHORT_FRAMES,          //!< SHORT_FRAMES
    NORMAL_FRAMES,         //!< NORMAL_FRAMES
    SHORT_AND_NORMAL_FRAMES//!< SHORT_AND_NORMAL_FRAMES
  } BbFrameUsageMode_t;

  /**
   * Compares to scheduling objects priorities
   *
   * \param obj1 First object to compare
   * \param obj2 Second object to compare
   * \return true if first object priority is considered to be higher that second object, false otherwise
   */
  static bool CompareSoFlowId (Ptr<SatSchedulingObject> obj1, Ptr<SatSchedulingObject> obj2);

  /**
   * Compares to scheduling objects priorities and load
   *
   * \param obj1 First object to compare
   * \param obj2 Second object to compare
   * \return true if first object priority is considered to be higher that second object or
   *         if priorities are same first object load is considered to be higher, false otherwise
   */
  static bool CompareSoPriorityLoad (Ptr<SatSchedulingObject> obj1, Ptr<SatSchedulingObject> obj2);

  /**
   * Compares to scheduling objects priorities and HOL
   *
   * \param obj1 First object to compare
   * \param obj2 Second object to compare
   * \return true if first object priority is considered to be higher that second object or
   *         if priorities are same first object HOL is considered to be higher, false otherwise
   */
  static bool CompareSoPriorityHol (Ptr<SatSchedulingObject> obj1, Ptr<SatSchedulingObject> obj2);

  static TypeId GetTypeId (void);

  /**
   * Construct a SatFwdLinkScheduler
   *
   * This the default constructor for the SatFwdLinkScheduler is not supported.
   *
   */
  SatFwdLinkScheduler ();

  /**
   * Actual constructor of a SatFwdLinkScheduler
   *    *
   * \param conf BB Frame configuration
   * \param address MAC address
   * \param Carrier bandwidth where scheduler is associated to [Hz].
   */
  SatFwdLinkScheduler (Ptr<SatBbFrameConf> conf, Mac48Address address, double carrierBandwidthInHz);

  /**
   * Destroy a SatFwdLinkScheduler
   *
   * This is the destructor for the SatFwdLinkScheduler.
   */
  ~SatFwdLinkScheduler ();

  /**
   * Get next frame to be transmitted.
   *
   * \param packets Pointers to packets received.
   */
  virtual Ptr<SatBbFrame> GetNextFrame ();

  /**
   * Callback to get scheduling contexts from upper layer
   * \param vector of scheduling contexts
   */
  typedef Callback<std::vector< Ptr<SatSchedulingObject> > > SchedContextCallback;

  /**
   * Callback to notify upper layer about Tx opportunity.
   * \param Mac48Address address
   * \param uint32_t payload size in bytes
   * \param uint8_t Flow identifier
   * \return packet Packet to be transmitted to PHY
   */
  typedef Callback< Ptr<Packet>, uint32_t, Mac48Address, uint8_t, uint32_t& > TxOpportunityCallback;

  /**
   * Method to set Tx opportunity callback.
    * \param cb callback to invoke whenever a packet has been received and must
    *        be forwarded to the higher layers.
    *
    */
  void SetSchedContextCallback (SatFwdLinkScheduler::SchedContextCallback cb);

  /**
   * Method to set Tx opportunity callback.
   * \param cb callback to invoke whenever a packet has been received and must
   *        be forwarded to the higher layers.
   */
  void SetTxOpportunityCallback (SatFwdLinkScheduler::TxOpportunityCallback cb);

  /**
   * Called when UT's C/N0 estimation is updated.
   *
   * \param utAddress Address of the UT updated C/N0 info.
   * \param cnoEstimate Value of the estimated C/N0.
   */
  void CnoInfoUpdated (Mac48Address utAddress, double cnoEstimate);

private:
  typedef std::map<Mac48Address, Ptr<SatCnoEstimator> > CnoEstimatorMap_t;

  SatFwdLinkScheduler& operator = (const SatFwdLinkScheduler &);
  SatFwdLinkScheduler (const SatFwdLinkScheduler &);

  /**
   * Do dispose actions.
   */
  void DoDispose (void);

  /**
   * Schedule BB Frames.
   */
  void ScheduleBbFrames ();

  /**
   * Create short or normal frame according to byteCount, estimated C/N0 and
   * according to member #m_bbFrameUsageMode.
   *
   * \param cno Estimated C/N0 value.
   * \param byteCount byte count
   * \return Pointer to created frame.
   */
  Ptr<SatBbFrame> CreateFrame (double cno, uint32_t byteCount) const;

  /**
   * Check if given estimated C/N0 match with given frame.
   *
   * \param cno Estimated C/N0 value.
   * \param frame Frame to match
   * \return True if C/N0 match with frame false otherwise.
   */
  bool CnoMatchWithFrame (double cno, Ptr<SatBbFrame> frame) const;

  /**
   *
   * \param Scheduling object
   * \return C/N0 estimated for object. NAN, if estimate is not available.
   */
  double GetSchedulingObjectCno (Ptr<SatSchedulingObject> ob);

  /**
   *  Handles periodic timer timeouts.
   */
  void PeriodicTimerExpired ();

  /**
   * Gets scheduling object in sorted order according to configured sorting criteria.
   *
   * \return Scheduling object available for scheduling
   */
  std::vector< Ptr<SatSchedulingObject> > GetSchedulingObjects ();

  /**
   * Sorts given scheduling objects according to configured sorting criteria.
   *
   * \param so Scheduling objects to sort.
   */
  void SortSchedulingObjects (std::vector< Ptr<SatSchedulingObject> >& so);

  /**
   * Create estimator for the UT according to set attributes.
   * \return pointer to created estimator
   */
  Ptr<SatCnoEstimator> CreateCnoEstimator ();

  /**
   * MAC address of the this instance (node)
   */
  Mac48Address m_macAddress;

  /**
   * Random variable used in FWD link scheduling
   */
  Ptr<UniformRandomVariable> m_random;

  /**
   * The interval for periodic scheduling timer. Timer is evaluated by scheduling calls to PeriodicTimerExpired.
   */
  Time m_periodicInterval;

  /**
   * Configured BB Frame conf.
   */
  Ptr<SatBbFrameConf> m_bbFrameConf;

  /**
   * Threshold time of total transmissions in BB Frame container to trigger a scheduling round.
   */
  Time m_schedulingStartThresholdTime;

  /**
   * Threshold time of total transmissions in BB Frame container to stop a scheduling round.
   */
  Time m_schedulingStopThresholdTime;

  /**
   * Additional sorting criteria for scheduling objects received from LLC.
   */
  ScheduleSortingCriteria_t m_additionalSortCriteria;

  /**
   * BBFrame usage mode.
   */
  BbFrameUsageMode_t m_bbFrameUsageMode;

  /**
   * The container for BB Frames.
   */
  Ptr<SatBbFrameContainer> m_bbFrameContainer;

  /**
   * Callback to notify the txOpportunity to upper layer
   * Returns a packet
   * Attributes: payload in bytes
   */
  SatFwdLinkScheduler::TxOpportunityCallback m_txOpportunityCallback;

  /**
   * The scheduling context getter callback.
   */
  SatFwdLinkScheduler::SchedContextCallback m_schedContextCallback;

  /**
   * C/N0 estimator per UT.
   */
  CnoEstimatorMap_t m_cnoEstimatorContainer;

  /**
   * Mode used for C/N0 estimator.
   */
  SatCnoEstimator::EstimationMode_t m_cnoEstimatorMode;

  /**
   * Time window for C/N0 estimation.
   */
  Time m_cnoEstimationWindow;

  /**
   * Carrier bandwidth in hertz where scheduler is associated to.
   */
  double m_carrierBandwidthInHz;

 };

} // namespace ns3

#endif /* SAT_FWD_LINK_SCHEDULER_H */
