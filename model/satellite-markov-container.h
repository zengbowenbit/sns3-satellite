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
 * Author: Frans Laakso <frans.laakso@magister.fi>
 */
#ifndef SATELLITE_MARKOV_CONTAINER_H
#define SATELLITE_MARKOV_CONTAINER_H

#include "satellite-markov-model.h"
#include "satellite-markov-conf.h"
#include "geo-coordinate.h"
#include "ns3/vector.h"
#include "satellite-fading.h"
#include "satellite-loo-model.h"
#include "satellite-rayleigh-model.h"
#include "ns3/traced-callback.h"

namespace ns3 {

/**
 * \ingroup satellite
 *
 * \brief Markov fading model container
 */
class SatMarkovContainer : public SatFading
{
public:

  /**
   * \brief NS-3 function for type id
   * \return type id
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  SatMarkovContainer ();

  /**
   * \brief Constructor
   * \param markovConf Markov configuration object
   * \param looConf Loo configuration object
   * \param currentPosition current position
   */
  SatMarkovContainer (Ptr<SatMarkovConf> markovConf, SatFading::ElevationCallback elevation, SatFading::VelocityCallback velocity);

  /**
   * \brief Destructor
   */
  ~SatMarkovContainer ();

  /**
   * \brief Function for getting the fading
   * \param channeltype channel type
   * \return fading value
   */
  double DoGetFading (SatEnums::ChannelType_t channeltype);

  /**
   * \brief Function for unlocking the parameter set and state
   */
  void UnlockSetAndState ();

  /**
   * \brief Function for locking the parameter set and state
   * \param newSet new set
   * \param newState new state
   */
  void LockToSetAndState (uint32_t newSet, uint32_t newState);

  /**
   * \brief Function for locking the parameter set
   * \param newSet new set
   */
  void LockToSet (uint32_t newSet);

  /**
   * \brief Function for locking the parameter set and state to random values
   */
  void LockToRandomSetAndState ();

private:

  /**
   * \brief Markov model object
   */
  Ptr<SatMarkovModel> m_markovModel;

  /**
   * \brief Markoc model configuration
   */
  Ptr<SatMarkovConf> m_markovConf;

  /**
   * \brief Uplink fader
   */
  Ptr<SatFader> m_fader_up;

  /**
   * \brief Downlink fader
   */
  Ptr<SatFader> m_fader_down;

  /**
   * \brief Number of states available
   */
  uint32_t m_numOfStates;

  /**
   * \brief Number of parameter sets available
   */
  uint32_t m_numOfSets;

  /**
   * \brief Current parameter set
   */
  uint32_t m_currentSet;

  /**
   * \brief Current state
   */
  uint32_t m_currentState;

  /**
   * \brief Cooldown period length in time
   */
  Time m_cooldownPeriodLength;

  /**
   * \brief Minimum state change distance in meters
   */
  double m_minimumPositionChangeInMeters;

  /**
   * \brief Latest calculated uplink fading value
   */
  double m_latestCalculatedFadingValue_up;

  /**
   * \brief Latest calculated downlink fading value
   */
  double m_latestCalculatedFadingValue_down;

  /**
   * \brief Latest calculation time for uplink fading value
   */
  Time m_latestCalculationTime_up;

  /**
   * \brief Latest calculation time for downlink fading value
   */
  Time m_latestCalculationTime_down;

  /**
   * \brief Variable for disabling the parameter set change
   */
  bool m_enableSetLock;

  /**
   * \brief Variable for disabling state changes
   */
  bool m_enableStateLock;

  /**
   * \brief Node movement velocity
   */
  VelocityCallback m_velocity;

  /**
   * \brief Latest calculation time for state change
   */
  Time m_latestStateChangeTime;

  /**
   * \brief Current elevation value
   */
  ElevationCallback m_currentElevation;

  /**
   * \brief Defines whether the calculations should return the fading value in decibels or not
   */
  bool m_useDecibels;

  /**
   * \brief Fading trace function
   */
  TracedCallback< double,                     // time
                  SatEnums::ChannelType_t,    // channel type
                  double                      // fading value
                  >
     m_fadingTrace;

  /**
   * \brief Function for updating the state change probabilities
   * \param set parameter set
   */
  void UpdateProbabilities (uint32_t set);

  /**
   * \brief Function for evaluating state change
   */
  void EvaluateStateChange (SatEnums::ChannelType_t channelType);

  /**
   * \brief Function for calculating the fading value
   * \param channelType channel type
   * \return fading value
   */
  double CalculateFading (SatEnums::ChannelType_t channelType);

  /**
   * \brief Function for calculating the elevation
   * \return elevation value
   */
  double CalculateElevation ();

  /**
   * \brief Function for checking whether the cooldown period has passed
   * \param channelType channel type
   * \return has cooldown period passed
   */
  bool HasCooldownPeriodPassed (SatEnums::ChannelType_t channelType);

  /**
   * \brief Function for getting the cached fading values
   * \param channelType channel type
   * \return cached fading value
   */
  double GetCachedFadingValue (SatEnums::ChannelType_t channelType);

  /**
   * \brief Function for creating the Markov state faders
   */
  void CreateFaders (SatMarkovConf::MarkovFaderType_t faderType);

  /**
   * \brief Function for calculating the distance since latest state change position
   * \return distance
   */
  double CalculateDistanceSinceLastStateChange ();

};

} // namespace ns3

#endif /* SATELLITE_MARKOV_CONTAINER_H */