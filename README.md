# LLVM Visualizer

LLVM Visualizer is a web-based tool for analyzing and visualizing the LLVM Intermediate Representation (IR), Control Flow Graph (CFG), and Dominator Frontier of a user-provided program. The tool allows users to upload their code, process it using LLVM, and interactively explore the generated IR and visual representations of CFG and Dominator Frontier.

[Live Demo](https://llvm-visualizer-awehczdag8agaea3.centralus-01.azurewebsites.net/)

## Requirements

- Python3
- Flask
- LLVM 19.1

## Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/hankluo6/llvm-visualizer.git
   cd llvm-visualizer
   ```

2. Install dependencies:
   ```bash
   pip install flask
   ```

3. Start the Flask server:
   ```bash
   python app.py
   ```

4. Open the web browser and navigate to `http://localhost:5000`.

## How It Works
1. The user inputs C/C++ code into the web interface.
2. The Flask backend:
   * Processes the input using LLVM's clang and opt tools to generate LLVM IR.
   * Runs custom LLVM passes to compute the Dominator Frontier and CFG.
   * Sends the IR and graph data (in JSON format) to the frontend.
3. The frontend:
   * Displays the LLVM IR in a styled text area.
   * Uses Cytoscape.js to render the CFG and Dominator Frontier as interactive graphs.
   * Highlights corresponding LLVM IR when graph nodes are clicked.

## License
This project is licensed under the [MIT License](LICENSE).