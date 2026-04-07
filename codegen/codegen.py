import argparse
import os
import subprocess
import sys

from caseconverter import camelcase, macrocase, pascalcase, snakecase
from fyeah import f
from jinja2 import select_autoescape, exceptions, Environment, FileSystemLoader
from lxml import etree

# args

parser = argparse.ArgumentParser(
    description="A Jinja code generator that renders from XML."
)

parser.add_argument(
    "--input",
    required=True,
    metavar="FILE",
    help="XML input"
)

parser.add_argument(
    "--template_dir",
    required=True,
    metavar="DIR",
    help="base folder for Jinja templates"
)

parser.add_argument(
    "--one",
    default=[],
    nargs=2,
    action="append",
    metavar=("TEMPLATE", "OUTPUT"),
    help="job with one output for the top-level XML element"
)

parser.add_argument(
    "--many",
    default=[],
    nargs=2,
    action="append",
    metavar=("TEMPLATE", "OUTPUT"),
    help="job with an output for each child of the top-level XML element"
)

args = parser.parse_args()

# lxml

input_as_xml = etree.parse(args.input, etree.XMLParser(remove_comments=True))

def to_dict(element):
    return {
        "tag": element.tag,
        "props": element.attrib,
        "children": [to_dict(child) for child in element]
    }

input_as_dict = to_dict(input_as_xml.getroot())

# jinja

env = Environment(
    loader=FileSystemLoader(args.template_dir),
    autoescape=select_autoescape(),
    trim_blocks=True, # remove newline after jinja code blocks
    lstrip_blocks=True, # ignore indentation of jinja code blocks
    line_comment_prefix="##" # enable inline comments
)

env.filters["camelCase"] = camelcase
env.filters["PascalCase"] = pascalcase
env.filters["snake_case"] = snakecase
env.filters["CONST_CASE"] = macrocase

def namespace_to_path(namespace):
    if not namespace:
        return ""
    return namespace.replace("::", "/") + "/"

env.filters["ns_to_path"] = namespace_to_path

class CodegenError(exceptions.TemplateRuntimeError):
    pass

def raise_codegen_error(message):
    raise CodegenError(message)

env.globals["error"] = raise_codegen_error

# helpers

def make_parent_dir(file_name):
    if dir_name := os.path.dirname(file_name):
        os.makedirs(dir_name, exist_ok=True)

def format_content(content):
    return subprocess.run(["clang-format"], input=content, capture_output=True, text=True).stdout

def overwrite_if_different(file_name, content):
    try:
        with open(file_name, "r") as file:
            if file.read() == content:
                return
    except FileNotFoundError:
        pass

    with open(file_name, "w") as file:
        file.write(content)

# templates

def evaluate_pattern(pattern, element):
    props = element["props"]
    tag = element["tag"]

    return f(pattern)

try:
    # one
    for (template, output) in args.one:
        template = env.get_template(template)

        file = evaluate_pattern(output, input_as_dict)
        make_parent_dir(file)

        overwrite_if_different(
            file,
            format_content(template.render(input_as_dict))
        )

    # many
    for (template, output) in args.many:
        template = env.get_template(template)

        for element in input_as_dict["children"]:
            file = evaluate_pattern(output, element)
            make_parent_dir(file)

            overwrite_if_different(
                file,
                format_content(template.render(element))
            )

    sys.exit(0)

except Exception as e:
    frames = []
    traceback = sys.exc_info()[2]

    while traceback is not None:
        # TODO: this is a bit brittle
        if traceback.tb_frame.f_code.co_filename.startswith(args.template_dir):
            frames.append(traceback)
        traceback = traceback.tb_next

    for frame in frames:
        print(f"{frame.tb_frame.f_code.co_filename}:{frame.tb_lineno}")
    print(e)

sys.exit(1)

