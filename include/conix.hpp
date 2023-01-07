/* ConixPP - Command line interface building library for C++
 * Copyright (C) 2021 Stan Vlad <vstan02@protonmail.com>
 *
 * This file is part of Conix.
 *
 * ConixPP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CONIX_CONIX_HPP
#define CONIX_CONIX_HPP

#include <cstddef>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <functional>

namespace cnx {
  const std::string DEFAULT_ID = "--default";
  const std::string NOT_FOUND_ID = "*";

  struct app {
    std::string name;
    std::string version;
  };

  struct ctx {
    cnx::app app;
    std::size_t argc;
    const char** argv;
  };

  struct handler {
    std::function<void (const cnx::ctx*, void*)> target;
    void* payload;

    handler(): target(), payload() {}

    handler(const cnx::handler& other)
      : target(other.target), payload(other.payload) {}

    handler(std::function<void (const cnx::ctx*, void*)> target, void* payload = nullptr)
      : target(target), payload(payload) {}
  };

  struct option {
    std::vector<std::string> ids;
    std::string description;
    cnx::handler handler;

    option(const cnx::option& other)
      : ids(other.ids), description(other.description), handler(other.handler) {}

    option(std::vector<std::string> ids, cnx::handler handler)
      : ids(ids), description(), handler(handler) {}

    option(std::vector<std::string> ids, std::function<void (const cnx::ctx*, void*)> handle, void* payload = nullptr)
      : ids(ids), description(), handler(handle, payload) {}

    option(std::vector<std::string> ids, std::string description, cnx::handler handler)
      : ids(ids), description(description), handler(handler) {}

    option(std::vector<std::string> ids, std::string description, std::function<void (const cnx::ctx*, void*)> handle, void* payload = nullptr)
      : ids(ids), description(description), handler(handle, payload) {}
  };

  struct info {
    std::string option;
    std::string description;

    bool operator<(const cnx::info& other) const {
      return option < other.option;
    }
  };

  class cli {
    public:
      explicit cli(cnx::app app);
      cli(std::string name, std::string version): cnx::cli(cnx::app { name, version }) {}

      int run(std::size_t argc, const char** argv);

      cnx::cli& add(cnx::option option);
      cnx::cli& add(std::vector<cnx::option> options);

      const std::set<cnx::info>& details() const;
      std::size_t max_info_size() const;

    private:
      cnx::ctx _context;
      std::set<cnx::info> _details;
      std::map<std::string, cnx::handler> _handlers;
      std::size_t _max_info_size;

    private:
      void handle(std::string option);
  };
};

#endif // CONIX_CONIX_HPP
