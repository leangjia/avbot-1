/*
 * Copyright (C) 2013  microcai <microcai@fedoraproject.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#ifndef __JOKE_HPP_
#define __JOKE_HPP_

#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

#include "extension.hpp"

class joke : avbotextension
{
private:
	boost::shared_ptr<boost::posix_time::seconds> m_interval;
	boost::shared_ptr<boost::asio::deadline_timer> m_timer;

	typedef	boost::function<void (const boost::system::error_code &, std::string)> joke_handler_type;
	boost::function<void ( joke_handler_type ) > m_async_jokefecher;

	void start();
	void set_joke_fecher();
	void load_setting();
	void save_setting();
public:
	template<class MsgSender>
	joke(boost::asio::io_service & _io_service, MsgSender sender, std::string channel_name, boost::posix_time::seconds interval = boost::posix_time::seconds(3600))
	  : avbotextension(_io_service, sender, channel_name),
		m_timer(new boost::asio::deadline_timer(_io_service)), m_interval(new boost::posix_time::seconds(interval))
	{
		load_setting();
		set_joke_fecher();
		start();
	}

	template<class MsgSender, class AsyncJokeFetcher>
	joke(boost::asio::io_service & _io_service, MsgSender sender, AsyncJokeFetcher _async_jokefecher, std::string channel_name, boost::posix_time::seconds interval = boost::posix_time::seconds(3600))
	  : avbotextension(_io_service, sender, channel_name),
	    m_timer(new boost::asio::deadline_timer(_io_service)), m_async_jokefecher(_async_jokefecher), m_interval(new boost::posix_time::seconds(interval))
	{
		load_setting();
		start();
	}

	void operator()(const boost::system::error_code& error);
	void operator()(boost::property_tree::ptree);
	void operator()(const boost::system::error_code& error, std::string joke);
};

#endif // __JOKE_HPP_
