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
#include <cmath>
#include "rcppsw/math/expression.hpp"
#include "rcppsw/task_allocation/time_estimate.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, task_allocation);

/*******************************************************************************n
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Calculates the probability that a robot will abort the task it is
 * currently working on.
 *
 * Depends on:
 *
 * - The reactivity parameter: How sensitive should robots be to abrupt changes
 *   in task estimates/execution times.
 * - Time estimates of the unpartitioned and two partitioned tasks.
 * - How long the robot has spent executing the current task.
 * - The offset parameter: Another parameter whose purpose I'm not quite sure
 *   of.
 */
class abort_probability: public rcppsw::math::expression<double> {
 public:
  abort_probability(double reactivity, double offset) :
      m_reactivity(reactivity), m_offset(offset) {}

  double calc(double exec_time,
              const time_estimate& whole_task,
              const time_estimate& subtask1,
              const time_estimate& subtask2) {
    double omega = m_reactivity * ((exec_time - whole_task.last_result())/
                                    (subtask1.last_result() + subtask2.last_result()) + m_offset);
    return set_result(1/(1 + std::exp(omega)));
  }

 private:
  double m_reactivity;
  double m_offset;
};


NS_END(task_allocation, rcppsw);

#endif /* INCLUDE_RCPPSW_TASK_ALLOCATION_ABORT_PROBABILITY_HPP_ */
