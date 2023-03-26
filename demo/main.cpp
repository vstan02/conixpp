/* Handling of one command line argument
 * Usage examples:
 * $ ./demo
 * $ ./demo -v
 * $ ./demo -p
 * $ ./demo --help
 */

#include <iostream>
#include <conix.hpp>

static void index_option(const cnx::ctx* ctx, void* payload);
static void about_option(const cnx::ctx* ctx, void* payload);
static void version_option(const cnx::ctx* ctx, void* payload);
static void print_args_option(const cnx::ctx* ctx, void* payload);
static void not_found_option(const cnx::ctx* ctx, void* payload);

extern int main(int argc, const char** argv) {
	return cnx::cli("my_app", "2.8.1")
		.add({ // Adding some options for handling:
			// cnx::option -> { name, description, handler, payload }
			{	{ "-a", "--about" }, "Display something", about_option },
			{ { "-p", "--print" }, "Display passed arguments", print_args_option },

			// Handlers for "-v, --version" and "-h, --help" are added
			// by default, but you can add your own handlers for them.
			{ { "-v", "--version" }, "Display version", version_option },

			// Handler for "--default" is called when are no arguments passed
			{ { "--default" }, "Default option", index_option },

			// Handler for "*" is called when is passed an unknown cli option.
			// By default it has the same handler as the "-h, --help" option.
			{ { "*" }, not_found_option }
		})
		.run(argc, argv);
}

static void index_option(const cnx::ctx* ctx, [[maybe_unused]] void* payload) {
	std::cout << "Welcome to " << ctx->app.name << "!\n";
}

static void about_option(const cnx::ctx* ctx, [[maybe_unused]] void* payload) {
	std::cout << ctx->app.name << " is a simple app created with conix!\n";
}

static void version_option(const cnx::ctx* ctx, [[maybe_unused]] void* payload) {
	std::cout << ctx->app.name << " -> " << ctx->app.version << '\n';
}

static void print_args_option(const cnx::ctx* ctx, [[maybe_unused]] void* payload) {
	std::cout << "Total arguments: " << ctx->argc << '\n';
	for (std::size_t i = 0; i < ctx->argc; ++i) {
		std::cout << i << " -> " << ctx->argv[i] << '\n';
	}
}

static void not_found_option(const cnx::ctx* ctx, [[maybe_unused]] void* payload) {
	std::cout << ctx->app.name << ": Invalid option!\n";
}
