/**
 * @file options.h
 *
 * @brief Compiler command line arguments and settings
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <defs.h>

/**
 * @brief Compiler command line options
 */
struct CCArgs {
    bool  emit_device;   //!< Whether to emit PTX code
    bool  print_ast;     //!< Whether to print the AST
    char *in_file;       //!< Input file, possibly null to indicate stdin
    char *out_file;      //!< Output file, possibly null to indicate stdout
    bool  verbose;       //!< Whether to print verbose output
    char *symbol_prefix; //!< String to prefix intern function names with
};

/**
 * @brief Get the options struct. This is not valid until parseArgs() is called.
 */
struct CCArgs *getOptions();

/**
 * @brief Print help message
 *
 * @param[in] argv Command line arguments
 */
void printHelp(char *argv[]);

/**
 * @brief Parse command line arguments. May terminate the program.
 *
 * @param[in] argc Number of command line arguments
 * @param[in] argv Command line arguments
 */
void parseArgs(int argc, char *argv[]);

#endif
