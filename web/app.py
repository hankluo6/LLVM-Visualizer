from flask import Flask, request, jsonify, send_from_directory, send_file
import subprocess
import os
import json

app = Flask(__name__)

CLANG_PATH = "clang"
OPT_PATH = "opt"

OUTPUT_DIR = "output"
os.makedirs(OUTPUT_DIR, exist_ok=True)

@app.route('/')
def index():
    return send_from_directory('.', './static/index.html')

@app.route('/submit', methods=['POST'])
def submit_code():
    data = request.json
    code = data.get('code', '')

    if not code.strip():
        return jsonify({"error": "No code provided"}), 400

    input_file = os.path.join(OUTPUT_DIR, "input.c")
    with open(input_file, "w+") as f:
        f.write(code)

    ir_file = os.path.join(OUTPUT_DIR, "output.ll")
    compile_cmd = [CLANG_PATH, "-emit-llvm", "-S", "-Xclang", "-disable-O0-optnone", input_file, "-o", ir_file]
    subprocess.run(compile_cmd, check=True)

    json_file = os.path.join(OUTPUT_DIR, "cfg.json")
    opt_cmd = [OPT_PATH, "-S", "-load-pass-plugin=../build/libllvm_graph_analysis.dylib", "-passes=function(graph-analysis)", ir_file, "-o", ir_file]
    subprocess.run(opt_cmd, check=True)

    with open(json_file, "r") as f:
        graph_data = json.load(f)

    return jsonify(graph_data)

@app.route('/llvm-ir', methods=['GET'])
def get_llvm_ir():
    try:
        file_path = 'output/output.ll'
        return send_file(file_path, mimetype='text/plain')
    except Exception as e:
        return jsonify({"error": str(e)}), 500
    
if __name__ == '__main__':
    app.run(debug=True)