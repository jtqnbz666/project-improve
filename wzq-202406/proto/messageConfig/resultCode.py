# -*- coding: utf-8 -*-
RESULT_CODE_DICT = {
    "SUCCESS": (200, "成功"),
    "ERROR": (500, "错误"),
    "EXIST_SAME_NAME": (1001, "存在相同昵称"),
}

DO_NOT_EDIT = "// Code generated by tool. DO NOT EDIT."

def gen_result_code_name(name):
    return f"RESULT_CODE_{name}"

def generate_result_code_header():
    code = []

    code.append(DO_NOT_EDIT)
    code.append("#ifndef RESULT_CODE_H")
    code.append("#define RESULT_CODE_H")
    code.append("")

    for name, (code_val, comment) in RESULT_CODE_DICT.items():
        code.append(f"const int {gen_result_code_name(name)} = {code_val}; // {comment}")

    code.append("")
    code.append("#endif // RESULT_CODE_H")

    save("ResultCode.h", code)

def save(filename, code):
    with open(filename, "w") as f:
        f.write("\n".join(code))
        print(f"{filename} has been generated.")

if __name__ == "__main__":
    generate_result_code_header()
