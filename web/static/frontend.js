const graph = {};
function submitCode() {
    const code = document.getElementById("input-code").value;
    if (!code.trim()) {
        alert("Please enter some code!");
        return;
    }

    fetch('/submit', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ code })
    })
        .then(response => response.json())
        .then(data => {
            load_llvm();
            if (data.cfg && data.dominator_tree) {
                renderGraph("cfg", data.cfg);
                renderGraph("dominator", data.dominator_tree);
            } else {
                alert("Error: Invalid response from server!");
            }
        })
        .catch(error => {
            console.error("Error:", error);
            alert("Failed to submit code. Please try again.");
        });
}

function load_llvm() {
    const llvmIRUrl = '/llvm-ir';

    fetch(llvmIRUrl)
        .then(response => {
            if (!response.ok) {
                throw new Error('Failed to load LLVM IR');
            }
            return response.text();
        })
        .then(data => {
            document.getElementById('llvm-content').textContent = data;
        })
        .catch(error => {
            console.error('Error loading LLVM IR:', error);
            document.getElementById('llvm-content').textContent = 'Error loading LLVM IR.';
        });
}

function renderGraph(containerId, graphData) {
    const elements = {};
    const nodes = [];
    const edges = [];
    graphData.forEach(node => {
        nodes.push({
            data: { id: node.name, label: node.label, dominator_frontier: node.dominator_frontier }
        });

        node.edge.forEach(target => {
            edges.push({
                data: { source: node.name, target }
            });
        });
    });

    elements.nodes = nodes;
    elements.edges = edges;

    var cy = cytoscape({
        container: document.getElementById(containerId),
        elements: elements,
        style: [
            {
                selector: 'node',
                css: {
                    'content': 'data(id)',
                    'text-valign': 'center',
                    'text-halign': 'center',
                    'background-color': '#000',
                    'color': '#fff',
                    'font-size': '12px',
                    'width': '30px',
                    'height': '30px',
                }
            },
            {
                selector: 'edge',
                css: {
                    'width': 2,
                    'line-color': '#888',
                    'target-arrow-color': '#888',
                    'target-arrow-shape': 'triangle',
                    'curve-style': 'bezier'
                }
            }
        ],
        layout: {
            name: 'breadthfirst',
            directed: true,
            padding: 10,
            avoidOverlap: true,
        },
        zoom: 1,
        minZoom: 0.5,
        maxZoom: 2,
    });
    graph[containerId] = cy;
    cy.on('mouseover', 'node', function (evt) {
        const mouseEvent = evt.originalEvent;
        if (mouseEvent) {
            showTooltip(mouseEvent.pageX, mouseEvent.pageY, this);
        }
    });

    cy.on('mouseout', 'node', function (evt) {
        hideTooltip(this);
    });

    cy.on('tap', 'node', function (evt) {
        hightlight(this, containerId);
    });

    function showTooltip(x, y, node) {
        let tooltip = document.getElementById('tooltip');
        if (!tooltip) {
            tooltip = document.createElement('div');
            tooltip.id = 'tooltip';
            document.body.appendChild(tooltip);
            tooltip.style.position = 'absolute';
            tooltip.style.background = '#fff';
            tooltip.style.border = '1px solid #ccc';
            tooltip.style.padding = '5px';
            tooltip.style.borderRadius = '3px';
            tooltip.style.boxShadow = '0 2px 4px rgba(0,0,0,0.2)';
            tooltip.style.zIndex = 1000;
        }

        tooltip.innerText = format(node.data('label'));
        tooltip.style.left = `${x + 10}px`;
        tooltip.style.top = `${y + 10}px`;
        tooltip.style.display = 'block';
    }

    function hideTooltip(node) {
        let tooltip = document.getElementById('tooltip');
        if (tooltip) {
            tooltip.style.display = 'none';
        }
    }

    function hightlight(node, currentContainerId) {
        if (node.style('background-color') == '#f00') {
            node.style('background-color', '#000');
            unhighlightLLVMIR();
            return;
        }
        node.style('background-color', '#f00');
        hightlightLLVMIR(node.data('label'));
        var df = node.data('dominator_frontier');
        cy.nodes().forEach(function (ele) {
            if (df.includes(ele.data('id'))) {
                ele.style('background-color', '#ffeb3b');
            } else if (ele != node) {
                ele.style('background-color', '#000');
            }
        });
        if (currentContainerId == 'cfg') {
            graph['dominator'].nodes().forEach(function (ele) {
                ele.style('background-color', '#000');
            });
        } else {
            graph['cfg'].nodes().forEach(function (ele) {
                ele.style('background-color', '#000');
            });
        }
    }

    function unhighlightLLVMIR() {
        const llvmContent = document.getElementById('llvm-content');
        const lines = llvmContent.textContent.split('\n');
        const highlightedContent = lines.map(line => line);
        llvmContent.innerHTML = highlightedContent.join('\n');
    }

    function hightlightLLVMIR(label) {
        const llvmContent = document.getElementById('llvm-content');
        const lines = llvmContent.textContent.split('\n');
        const IR = label.split(/\\l/g);
        const firstInstruction = IR[0];
        const lastInstruction = IR[IR.length - 2];
        const lineNumbers = []
        for (let i = 0; i < lines.length; i++) {
            if (lines[i].includes(firstInstruction)) {
                lineNumbers.push(i);
            }
            if (lineNumbers.length == 1 && lines[i].includes(lastInstruction)) {
                lineNumbers.push(i);
                break;
            }
        }
        const highlightedLines = [];
        for (let i = lineNumbers[0] - 1; i <= lineNumbers[1]; i++) {
            highlightedLines.push(i);
        }
        const highlightedContent = lines.map((line, index) => {
            if (highlightedLines.includes(index)) {
                return `<span style="background-color: #ffeb3b">${line}</span>`;
            }
            return line;
        });
        llvmContent.innerHTML = highlightedContent.join('\n');
    }

    function format(label) {
        return label.replace(/\\l/g, '\n');
    }
}
