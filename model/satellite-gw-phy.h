/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions Ltd.
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
 * Author: Sami Rantanen <sami.rantanennen@magister.fi>
 */

#ifndef SATELLITE_GW_PHY_H
#define SATELLITE_GW_PHY_H

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "satellite-phy.h"
#include "satellite-signal-parameters.h"

namespace ns3 {


/**
 * \ingroup satellite
 *
 * The SatGwPhy models the physical layer of the satellite system (UT, GW, satellite)
 */
class SatGwPhy : public SatPhy
{
public:

  /**
   * Default constructor
   */
  SatGwPhy (void);

  SatGwPhy (SatPhy::CreateParam_t& params, ErrorModel errorModel, Ptr<SatLinkResults> linkResults,
            InterferenceModel ifModel, CarrierBandwidthConverter converter, uint32_t carrierCount);

  virtual ~SatGwPhy ();

  // inherited from Object
  static TypeId GetTypeId (void);
  TypeId GetInstanceTypeId (void) const;
  virtual void DoStart (void);
  virtual void DoDispose (void);

  /**
   * GW specific SINR calculator.
   * Calculate SINR with GW PHY specific parameters and given SINR.
   *
   * \param sinr Calculated (C/NI)
   */
  virtual double CalculateSinr (double sinr);

private:

  /**
   *  Configured adjacent channel interference wrt noise (percent).
   */
  double m_aciIfWrtNoisePercent;

  /**
   * Configured intermodulation interference in dB.
   */
  double m_imInterferenceCOverIDb;

  /**
   * Intermodulation interference in linear.
   */
  double m_imInterferenceCOverI;
};

}

#endif /* SATELLITE_GW_PHY_H */