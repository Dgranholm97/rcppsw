/**
 * @file identifiable.hpp
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

#ifndef INCLUDE_RCPPSW_COMMON_IDENTIFIABLE_HPP_
#define INCLUDE_RCPPSW_COMMON_IDENTIFIABLE_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/common/common.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, common);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class identifiable {
 public:
  identifiable(void) {}
  virtual ~identifiable(void) {}

  virtual identifiable* identify(void) = 0;
};

NS_END(common, rcppsw);

#endif /* INCLUDE_RCPPSW_COMMON_IDENTIFIABLE_HPP_ */
