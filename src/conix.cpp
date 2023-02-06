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

#include <cstdio>

#include "conix.hpp"

static void help(const cnx::ctx* ctx, void* payload);
static void version(const cnx::ctx* ctx, void* payload);

extern cnx::cli::cli(cnx::app app)
	: _context({ app, 0, nullptr })
	, _details()
	, _handlers()
	, _max_info_size(0) {
	add({
		{ { "-h", "--help" }, "Display this information", help, static_cast<void*>(this) },
		{ { "-v", "--version" }, "Display version information", version },
		{ { "*" }, help, static_cast<void*>(this) },
	});
}

extern int cnx::cli::run(std::size_t argc, const char** argv) {
	if (argc > 1) {
		_context.argc = argc - 1;
		_context.argv = argv + 1;
	}
	handle(argc > 1 ? argv[1] : cnx::DEFAULT_ID);
	return 0;
}

extern cnx::cli& cnx::cli::add(cnx::option option) {
	std::string name;
	for (std::string& id: option.ids) {
		_handlers[id] = option.handler;
		if (id != cnx::NOT_FOUND_ID) {
			name += name.empty() ? id : (", " + id);
		}
	}

	if (name.size() > _max_info_size) {
		_max_info_size = name.size();
	}
	if (!name.empty()) {
		_details.insert({ name, option.description });
	}
	return *this;
}

extern cnx::cli& cnx::cli::add(std::vector<cnx::option> options) {
	for (cnx::option option: options) {
		add(option);
	}
	return *this;
}

extern void cnx::cli::handle(std::string option) {
	auto result = _handlers.find(option);
	if (result != _handlers.end()) {
		return result->second.target(&_context, result->second.payload);
	}
	result = _handlers.find(cnx::NOT_FOUND_ID);
	if (result != _handlers.end()) {
		return result->second.target(&_context, result->second.payload);
	}
}

extern const std::set<cnx::info>& cnx::cli::details() const {
	return _details;
}

extern std::size_t cnx::cli::max_info_size() const {
	return _max_info_size;
}

static void help(const cnx::ctx* ctx, void* payload) {
	std::printf("Usage: %s [options]\n\nOptions:\n", ctx->app.name.c_str());

	cnx::cli* cli = static_cast<cnx::cli*>(payload);
	int size = -(int)cli->max_info_size();
	for (const cnx::info& info: cli->details()) {
		std::printf("    %*s  %s\n", size, info.option.c_str(), info.description.c_str());
	}
}

static void version(const cnx::ctx* ctx, void* payload) {
	std::printf("%s v%s\n", ctx->app.name.c_str(), ctx->app.version.c_str());
}
