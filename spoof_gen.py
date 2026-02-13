import os
import random
import struct

# 1. Define the Signatures (Magic Numbers) - Expanded to match C tool
signatures = {
    "ZIP":  b'\x50\x4B\x03\x04',
    "PDF":  b'\x25\x50\x44\x46',
    "GIF":  b'\x47\x49\x46\x38',
    "PNG":  b'\x89\x50\x4E\x47\x0D\x0A\x1A\x0A',
    "EXE":  b'\x4D\x5A',
    "JPEG": b'\xFF\xD8\xFF',
    "ELF":  b'\x7F\x45\x4C\x46',
    "JAVA": b'\xCA\xFE\xBA\xBE',
    "MP3":  b'\x49\x44\x33',
    "BMP":  b'\x42\x4D',
    "RAR":  b'\x52\x61\x72\x21\x1A\x07',
    "7Z":   b'\x37\x7A\xBC\xAF\x27\x1C',
    "GZIP": b'\x1F\x8B',
    "FLAC": b'\x66\x4C\x61\x43',
    "MIDI": b'\x4D\x54\x68\x64',
    "TIFF_LE": b'\x49\x49\x2A\x00',
    "TIFF_BE": b'\x4D\x4D\x00\x2A',
    "RTF":  b'\x7B\x5C\x72\x74\x66\x31'
}

# 2. Define Mismatched Extensions for Testing
# format: (Real Type, Fake Extension to apply, Entropy Strategy)
# Entropy Strategy: 'low', 'medium', 'high'
test_cases = [
    ("ZIP", "txt", "high"),
    ("PDF", "jpg", "medium"),
    ("GIF", "pdf", "medium"),
    ("PNG", "docx", "high"),
    ("EXE", "png", "high"),
    ("JPEG", "gif", "high"),
    ("ELF", "txt", "high"),
    ("JAVA", "class.txt", "medium"),
    ("MP3", "wav", "high"),
    ("BMP", "jpg", "low"),
    ("RAR", "iso", "high"),
    ("7Z", "zip", "high"),
    ("RTF", "txt", "low")
]

def generate_content(strategy, size=16384):
    """Generates content with varying entropy levels."""
    if strategy == "low":
        # Repeating pattern - very low entropy
        pattern = b"A" * 10 + b"B" * 10
        return (pattern * (size // len(pattern) + 1))[:size]
    elif strategy == "medium":
        # Text-like content - medium entropy
        chars = b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 \n"
        return bytes(random.choice(chars) for _ in range(size))
    elif strategy == "high":
        # Random bytes - high entropy
        return os.urandom(size)
    return b'\x00' * size

def generate_test_files():
    output_dir = "test_files"
    
    # Create directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"[+] Created directory: {output_dir}")

    print(f"[+] Generating {len(test_cases)} false-type files with entropy variations...\n")
    print(f"{'Filename':<35} | {'Real Header':<12} | {'Fake Ext':<10} | {'Entropy':<8}")
    print("-" * 75)

    for real_type, fake_ext, entropy_strat in test_cases:
        # Construct the filename: realtype_disguised_as.fake_ext
        filename = f"real_{real_type}_spoofed.{fake_ext}"
        filepath = os.path.join(output_dir, filename)
        
        # Get the correct magic bytes
        magic_bytes = signatures.get(real_type)
        
        if magic_bytes:
            with open(filepath, 'wb') as f:
                # Write the magic bytes at the very start of the file
                f.write(magic_bytes)
                
                # Generate payload (16KB to ensure windowed entropy works)
                payload = generate_content(entropy_strat, size=16384)
                f.write(payload)
            
            print(f"{filename:<35} | {real_type:<12} | .{fake_ext:<9} | {entropy_strat}")
        else:
            print(f"    [!] Error: Signature for {real_type} not found.")

    print(f"\n[+] Done. Check the '{output_dir}' folder.")

if __name__ == "__main__":
    generate_test_files()