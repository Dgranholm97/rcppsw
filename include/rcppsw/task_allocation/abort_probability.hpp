/**
 * @file abort_probability.hpp
 *
 * @copyright 2017 John Harwell, All rights reserved.
 *
 * This file is part of RCPPSW.
 *
 * RCPPSW is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * RCPPSW is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * RCPPSW.  If not, see <http://www.gnu.org/licenses/
 */

#ifndef INCLUDE_RCPPSW_TASK_ALLOCATION_ABORT_PROBABILITY_HPP_
#define INCLUDE_RCPPSW_TASK_ALLOCATION_ABORT_PROBABILITY_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/math/expression.hpp"
#include "rcppsw/task_allocation/time_estimate.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, task_allocation);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class abort_probability
 * @ingroup task_allocation
 *
 * @brief Calculates the probability that a robot will abort the task it is
 * currently working on using the negative exponential distribution.
 *
 * Reactivity and offset are assumed to both be > 0.
 *
 * Depends on:
 *
 * - The reactivity parameter: How sensitive should robots be to abrupt changes
 *   in task estimates/execution times?
 *
 * - The offset parameter: How much the current_exec/prev_estimate ratio will be
 *   allowed to grow before causing the probability to grow exponentially.
 *
 * - A time estimate for the task (can execution time thus far, interface time
 *   thus far, etc.).
 */
class abort_probability : public rcppsw::math::expression<double> {
 public:
  abort_probability(double reactivity, double offset)
      : m_reactivity(reactivity), m_offset(offset) {}

  double calc(double exec_time, const time_estimate& whole_task);

  /*
   * @brief If we don't have any time estimate for the task, then we just set a
   * small static abort probability.
   */
  static double constexpr kNO_EST_ABORT_PROB = 0.001;

 private:
  double m_reactivity;
  double m_offset;
};

NS_END(task_allocation, rcppsw);

#endif /* INCLUDE_RCPPSW_TASK_ALLOCATION_ABORT_PROBABILITY_HPP_ */
