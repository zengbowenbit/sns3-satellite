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
 * Author: Frans Laakso <frans.laakso@magister.fi>
 */

#include "satellite-loo-conf.h"
#include "satellite-markov-conf.h"
#include <map>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SatLooConf);
NS_LOG_COMPONENT_DEFINE ("SatLooConf");

static const double g_LooParameters[SatMarkovConf::DEFAULT_ELEVATION_COUNT][SatMarkovConf::DEFAULT_STATE_COUNT][SatLooConf::DEFAULT_LOO_PARAMETER_COUNT] =
    {
     /* Elevation 40 */  // TODO: add Ka-band parameters
     {{-0.1,0.37,-22.0,10,10,2,30},
      {-1.0,0.5,-22.0,10,10,2,30},
      {-2.25,0.13,-21.2,10,10,2,30}},

     /* Elevation 60 */
     {{0.0,0.12,-24.9,10,10,2,30},
      {-0.7,0.12,-26.1,10,10,2,30},
      {-1.4,0.25,-23.1,10,10,2,30}},

     /* Elevation 70 */
     {{-0.1,0.25,-22.5,10,10,2,30},
      {-0.5,0.28,-24.5,10,10,2,30},
      {-0.75,0.37,-23.24,10,10,2,30}},

     /* Elevation 80 */
     {{0.1,0.16,-22.4,10,10,2,30},
      {-0.4,0.15,-23.5,10,10,2,30},
      {-0.72,0.27,-22.0,10,10,2,30}}
    };

TypeId
SatLooConf::GetTypeId (void) //TODO: add attribute m_looParameters
{
  static TypeId tid = TypeId ("ns3::SatLooConf")
      .SetParent<SatFaderConf> ()
      .AddConstructor<SatLooConf> ()
      .AddAttribute ("ElevationCount", "Number of elevation sets in the Markov model.",
                     UintegerValue (SatMarkovConf::DEFAULT_ELEVATION_COUNT),
                     MakeUintegerAccessor (&SatLooConf::m_elevationCount),
                     MakeUintegerChecker<uint32_t> ())
      .AddAttribute ("StateCount", "Number of states in the Markov model.",
                     UintegerValue (SatMarkovConf::DEFAULT_STATE_COUNT),
                     MakeUintegerAccessor (&SatLooConf::m_stateCount),
                     MakeUintegerChecker<uint32_t> ());
  return tid;
}

SatLooConf::SatLooConf () :
    m_elevationCount (SatMarkovConf::DEFAULT_ELEVATION_COUNT),
    m_stateCount (SatMarkovConf::DEFAULT_STATE_COUNT)
{
  NS_LOG_FUNCTION (this);

  NS_LOG_INFO ("Time " << Now ().GetSeconds () << " SatLooConf - Creating SatLooConf...");

  for (uint32_t i = 0; i < m_elevationCount; i++)
    {
      std::vector<std::vector<double> > states;

      for (uint32_t j = 0; j < m_stateCount; j++)
        {
          std::vector<double> parameters;

          for (uint32_t k = 0; k < SatLooConf::DEFAULT_LOO_PARAMETER_COUNT; k++)
            {
              parameters.push_back (g_LooParameters[i][j][k]);
            }
          states.push_back (parameters);
        }
      m_looParameters.push_back (states);
    }
}

std::vector<std::vector<double> >
SatLooConf::GetParameters (uint32_t set)
{
  NS_LOG_FUNCTION (this << set);

  NS_ASSERT ( (set >= 0) && (set < m_elevationCount));
  NS_LOG_INFO ("Time " << Now ().GetSeconds () << " SatLooConf - Getting Loo parameters for set ID " << set);
  return m_looParameters[set];
}

} // namespace ns3

