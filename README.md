# LLVM Visualizer

LLVM Visualizer is a web-based tool for analyzing and visualizing the LLVM Intermediate Representation (IR), Control Flow Graph (CFG), and Dominator Frontier of a user-provided program. The tool allows users to upload their code, process it using LLVM, and interactively explore the generated IR and visual representations of CFG and Dominator Frontier.

## Requirements

- Python 3
- Flask
- LLVM

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

## License
This project is licensed under the MIT License.