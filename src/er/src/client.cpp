/**
 * @file client.cpp
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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/er/client.hpp"
#include "rcppsw/er/server.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, er);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
client::client(const std::shared_ptr<server>& server_handle)
    : m_server_handle(server_handle), m_er_id(m_server_handle->idgen()) {}
client::client(void) : m_server_handle(), m_er_id() {}
client::~client(void) {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
status_t client::attmod(const std::string& mod_name) {
  return server_handle()->findmod(mod_name, m_er_id);
} /* attmod() */

void client::deferred_init(const std::shared_ptr<server>& server_handle) {
  m_server_handle = server_handle;
  m_er_id = m_server_handle->idgen();
} /* deferred_init() */

void client::change_id(boost::uuids::uuid old_id, boost::uuids::uuid new_id) {
  m_server_handle->change_id(old_id, new_id);
} /* change_id() */

status_t client::insmod(const std::string& mod_name,
                           const er_lvl::value& loglvl,
                           const er_lvl::value& dbglvl) {
  return m_server_handle->insmod(m_er_id, loglvl, dbglvl, mod_name);
} /* insmod */

status_t client::rmmod(void) { return m_server_handle->rmmod(m_er_id); }

void __er_report__(server* server, const boost::uuids::uuid& er_id,
                    const er_lvl::value& lvl, const std::string& str) {
  server->report(er_id, lvl, str);
} /* __er_report__() */

NS_END(er, rcppsw);
