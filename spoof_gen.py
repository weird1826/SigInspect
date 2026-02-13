import os

# 1. Define the Signatures (Magic Numbers)
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
    "BMP":  b'\x42\x4D'
}

# 2. Define Mismatched Extensions for Testing
# format: (Real Type, Fake Extension to apply)
test_cases = [
    ("ZIP", "txt"),   # Real ZIP, looks like .txt
    ("PDF", "jpg"),   # Real PDF, looks like .jpg
    ("GIF", "pdf"),   # Real GIF, looks like .pdf
    ("PNG", "docx"),  # Real PNG, looks like .docx
    ("EXE", "png"),   # Real EXE, looks like .png (Common malware technique)
    ("JPEG", "gif"),  # Real JPEG, looks like .gif
    ("ELF", "txt"),   # Real ELF, looks like .txt
    ("JAVA", "class.txt"), # Real Java Class, looks like .txt
    ("MP3", "wav"),   # Real MP3, looks like .wav
    ("BMP", "jpg")    # Real BMP, looks like .jpg
]

def generate_test_files():
    output_dir = "test_files"
    
    # Create directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"[+] Created directory: {output_dir}")

    print(f"[+] Generating {len(test_cases)} false-type files...\n")

    for real_type, fake_ext in test_cases:
        # Construct the filename: realtype_disguised_as.fake_ext
        filename = f"real_{real_type}_spoofed.{fake_ext}"
        filepath = os.path.join(output_dir, filename)
        
        # Get the correct magic bytes
        magic_bytes = signatures.get(real_type)
        
        if magic_bytes:
            with open(filepath, 'wb') as f:
                # Write the magic bytes at the very start of the file
                f.write(magic_bytes)
                
                # Add some padding/dummy data so the file isn't empty
                f.write(b'\x00' * 20) 
                
                # Add a text string to confirm readable content if opened in hex editor
                f.write(f" This is actually a {real_type} file.".encode('utf-8'))
            
            print(f"    Created: {filename:<30} | Header: {real_type} | Ext: .{fake_ext}")
        else:
            print(f"    [!] Error: Signature for {real_type} not found.")

    print(f"\n[+] Done. Check the '{output_dir}' folder.")

if __name__ == "__main__":
    generate_test_files()