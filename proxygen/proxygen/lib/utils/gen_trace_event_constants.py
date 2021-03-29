#!/bin/env python3
# Copyright (c) Facebook, Inc. and its affiliates.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.

from __future__ import absolute_import, division, print_function, unicode_literals

import optparse
import os
import re
import sys


def main(argv):
    # args parser
    parser = optparse.OptionParser()
    parser.add_option(
        "--install_dir",
        dest="install_dir",
        type="string",
        default=None,
        help="Absolute path to generate files",
    )
    parser.add_option(
        "--fbcode_dir",
        dest="fbcode_dir",
        type="string",
        default=None,
        help="Absolute path to fbcode directory",
    )
    parser.add_option(
        "--input_files",
        dest="input_files",
        type="string",
        default=None,
        help="Relative path of input file",
    )
    parser.add_option(
        "--output_scope",
        dest="output_scope",
        type="string",
        default=None,
        help="namespace / package of output file",
    )
    parser.add_option(
        "--output_type",
        dest="output_type",
        type="choice",
        choices=["java", "cpp"],
        default=None,
        help="File type to generate",
    )
    parser.add_option(
        "--header_path",
        dest="header_path",
        type="string",
        default=None,
        help="Relative path to cpp header",
    )
    options, _ = parser.parse_args()

    assert options.install_dir is not None, "Missing arg: --install_dir"
    assert options.fbcode_dir is not None, "Missing arg: --fbcode_dir"
    assert options.input_files is not None, "Missing arg: --input_files"
    assert options.output_scope is not None, "Missing arg: --output_scope"
    assert options.output_type is not None, "Missing arg: --output_type"

    file_names = options.input_files.split(",")
    for file_name in file_names:
        # strip the file extension and use the file name for class name
        class_name = os.path.basename(file_name).split(".")[0]

        # parse items from source
        items = []
        with open(file_name, "r") as inf:
            for line in inf:
                sp = re.match(r"(.*), \"(.*)\"", line, re.I)
                if sp:
                    items.append((sp.group(1), sp.group(2)))

        if options.output_type == "java":
            gen_java(items, class_name, options.install_dir, options.output_scope)

        elif options.output_type == "cpp":
            assert options.header_path is not None, "Missing arg: --header_path"

            gen_cpp_header(items, class_name, options.install_dir, options.output_scope)
            gen_cpp_source(
                items,
                class_name,
                options.install_dir,
                options.header_path,
                options.output_scope,
            )


"""
Generate java interface class
"""


def gen_java(items, class_name, install_dir, output_scope):
    packages = output_scope.split(".")
    file_name = "%s.java" % class_name
    file_path = os.path.join(*([install_dir, "src"] + packages))
    output_file = os.path.join(file_path, file_name)

    if not os.path.exists(file_path):
        os.makedirs(file_path)

    with open(output_file, "w+") as outf:
        outf.write("// Copyright 2015-present Facebook. All Rights Reserved.\n")
        outf.write("// ** AUTOGENERATED FILE. DO NOT HAND-EDIT **\n\n")
        outf.write("package %s;\n\n" % ".".join(packages))
        outf.write("public interface %s {\n" % class_name)

        for item in items:
            outf.write(
                '    public static final String %s = "%s";\n' % (item[0], item[1])
            )

        outf.write("}\n")


"""
Generate cpp enum class and provide convert function from / to string
"""


def gen_cpp_header(items, class_name, install_dir, output_scope):
    namespaces = output_scope.split("::")
    file_name = "%s.h" % class_name
    output_file = os.path.join(install_dir, file_name)

    with open(output_file, "w+") as outf:
        outf.write("// Copyright 2015-present Facebook. All Rights Reserved.\n")
        outf.write("// ** AUTOGENERATED FILE. DO NOT HAND-EDIT **\n\n")
        outf.write("#pragma once\n\n")
        outf.write("#include <string>\n\n")
        for ns in namespaces:
            outf.write("namespace %s { " % ns)
        outf.write("\n\n")

        # generate enum class
        outf.write("enum class %s {\n" % class_name)
        for item in items:
            outf.write("    %s,\n" % item[0])
        outf.write("};\n\n")

        # enum to string convert function
        outf.write(
            "extern const std::string& get%sString(%s);\n" % (class_name, class_name)
        )

        outf.write(
            "extern %s get%sFromString(const std::string&);\n"
            % (class_name, class_name)
        )
        for _ in namespaces:
            outf.write("}")
        outf.write("\n\n")


"""
Generate cpp const string and implement convert function
"""


def gen_cpp_source(items, class_name, install_dir, header_path, output_scope):
    namespaces = output_scope.split("::")
    file_name = "%s.cpp" % class_name
    output_file = os.path.join(install_dir, file_name)

    with open(output_file, "w+") as outf:
        outf.write("// Copyright 2015-present Facebook. All Rights Reserved.\n")
        outf.write("// ** AUTOGENERATED FILE. DO NOT HAND-EDIT **\n\n")
        outf.write('#include "%s/%s.h"\n\n' % (header_path, class_name))
        outf.write("#include <stdexcept>\n\n")

        for ns in namespaces:
            outf.write("namespace %s { " % ns)
        outf.write("\n\n")

        # const string names
        for item in items:
            outf.write(
                'static const std::string k%s%s = "%s";\n'
                % (class_name, item[0], item[1])
            )

        # generate enum to string convert function
        outf.write(
            "const std::string& get%sString(%s type) {\n" % (class_name, class_name)
        )

        outf.write('  static const std::string k%sInvalidType = "";\n' % class_name)

        outf.write("\n  switch (type) {\n")
        for item in items:
            outf.write(
                "    case %s::%s : return k%s%s;\n"
                % (class_name, item[0], class_name, item[0])
            )
        outf.write("  }\n")
        outf.write("  return k%sInvalidType;\n" % class_name)
        outf.write("};\n\n")

        outf.write(
            "  %s get%sFromString(const std::string& str) {\n"
            % (class_name, class_name)
        )
        for item in items:
            outf.write(
                "  if (str == k%s%s)  return %s::%s;\n"
                % (class_name, item[0], class_name, item[0])
            )
        outf.write(
            " throw std::invalid_argument"
            ' ("No matching %s from string");\n' % (class_name)
        )
        outf.write("};\n")
        outf.write("\n\n")
        for _ in namespaces:
            outf.write("}")
        outf.write("\n\n")


if __name__ == "__main__":
    sys.exit(main(sys.argv))
