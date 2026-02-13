# SigInspect
SigInspect is a lightweight C-based file signature & entropy analysis tool.

## Overview
This tool analyzes the file signature, often referred to as magic number of a file, and deduces its actual file type by comparing the file signature to an in-built database of known signatures. After doing so, it calculates Shannon Entrony, which essentially helps us deduce if the file is potentically packed, encrypted, or obfuscated. It might be useful in malware analysis, digital forensics, and low-level system exploration.

## Features
* File Signature Detection
* Prints RAW Hex Header
* Shannon Entropy Calculation
* Suggestion of encrypted/sparse data

## Technical Explanation
### File Signature Analysis
* Reads first 64 bytes
* Compares against known magic numbers
* Identifies probable file type

### Entropy Analysis
* Calculates Shannon Entropy:
$$
H = -\sum p(x) \log_2 p(x)
$$
* High Entropy may indicate encrypted, compressed, or random data

## Installation
```bash
git clone https://github.com/weird1826/SigInspect.git

cd SigInspect

gcc siginspect.c -o siginspect
```

## Usage
```bash
./siginspect.exe <file_path>
```

### Example Output:
```
[*] File opened successfully!
[!] Analysis Results for: .\test_files\real_BMP_spoofed.jpg
        File Type (Signature): BMP Image
        Raw Header (Hex): 42 4D 41 41 41 41 41 41 41 41 41 41 42 42 42 42 
        Shannon Entropy: 1.0009 bits/byte
                Analysis: Low entropy. Likely text, padding, or sparse data.
```

## Limitations
* Limited signature Database
* Whole-file Entropy only

## Future Improvements
* Windows Entropy Analysis
* Expanded/External DB
* Output Mode
* YARA Rule Integration
* Multi-threaded Analysis
* Recursive directory scanning

## Extra Files
The project also has Python Script which generates spoofed files with varying entropy levels so that one can test the tool without searching for the test files.

## Use Case
This tool can be useful in Malware triage, Suspicious file detection, IR, Academic Research.