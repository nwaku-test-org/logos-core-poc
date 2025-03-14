#ifndef LOGOS_CORE_H
#define LOGOS_CORE_H

// Define export macro for the library
#if defined(LOGOS_CORE_LIBRARY)
#  define LOGOS_CORE_EXPORT __attribute__((visibility("default")))
#else
#  define LOGOS_CORE_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the logos core library
LOGOS_CORE_EXPORT void logos_core_init(int argc, char *argv[]);

// Set a custom plugins directory
LOGOS_CORE_EXPORT void logos_core_set_plugins_dir(const char* plugins_dir);

// Start the logos core functionality
LOGOS_CORE_EXPORT void logos_core_start();

// Run the event loop
LOGOS_CORE_EXPORT int logos_core_exec();

// Clean up resources
LOGOS_CORE_EXPORT void logos_core_cleanup();

// Get the list of loaded plugins
// Returns a null-terminated array of plugin names that must be freed by the caller
LOGOS_CORE_EXPORT char** logos_core_get_loaded_plugins();

#ifdef __cplusplus
}
#endif

#endif // LOGOS_CORE_H 