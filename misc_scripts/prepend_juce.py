import os
import re
import argparse


# Command-line argument parser
parser = argparse.ArgumentParser(description='Prepend juce:: to class names in source files.')
parser.add_argument('--quiet', action='store_true', help='Suppress detailed change outputs.')
args = parser.parse_args()

# Directory containing source files
source_dir = 'source'
# Directory to save modified files
output_dir = 'outd'

# Create output directory if it doesn't exist
os.makedirs(output_dir, exist_ok=True)

# Read class names from the file
with open('class_names.txt', 'r') as file:
    class_names = [line.strip() for line in file]

# Function to prepend 'juce::' to class names and save to a new file
def prepend_juce_to_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    full_content = ''.join(lines)  # Combine lines for processing

    # First pass: prepend 'juce::' to correct class names
    for class_name in class_names:
        # Regex pattern to avoid adding 'juce::' if already preceded by any word and '::'
        pattern = fr"(?<!\bjuce::)\b{class_name}\b"  
        new_content = re.sub(pattern, f"juce::{class_name}", full_content)

        
        # Update full_content for next checks
        full_content = new_content

    # Second pass: Check for any incorrect class names in the format 'word::juce::classname'
    incorrect_class_names = []
    for class_name in class_names:
        pattern = fr"\b[a-zA-Z_][\w]*::juce::{class_name}\b"  # Pattern to find incorrect class names
        if re.search(pattern, full_content):
            incorrect_class_names.append(class_name)

    # Write the modified content to a new file in the output directory
        # Preserve the directory structure
    relative_path = os.path.relpath(file_path, source_dir)
    output_file_path = os.path.join(output_dir, relative_path)
    os.makedirs(os.path.dirname(output_file_path), exist_ok=True)
    with open(output_file_path, 'w') as file:
        file.write(full_content)

    # Print changes if not in quiet mode
    if not args.quiet:
        print(f"Changes made in {output_file_path}:")
        for class_name in incorrect_class_names:
            print(f" - Incorrect class name found: {class_name}")

    return len(incorrect_class_names)

# Iterate over all files in the source directory
total_files = 0
for root, dirs, files in os.walk(source_dir):
    for file_name in files:
        if file_name.endswith('.cpp') or file_name.endswith('.h'):
            total_files += 1

# Process files and show progress
processed_files = 0
for root, dirs, files in os.walk(source_dir):
    for file_name in files:
        if file_name.endswith('.cpp') or file_name.endswith('.h'):
            file_path = os.path.join(root, file_name)
            prepend_juce_to_file(file_path)
            processed_files += 1
            remaining_files = total_files - processed_files
            if not args.quiet:
                print(f"Processed {processed_files}/{total_files} ({remaining_files} files left): {file_path}")
            elif processed_files % 10 == 0:  # Print progress every 10 files in quiet mode
                print(f"Processed {processed_files}/{total_files} files...")

print("Prepending completed. Changes saved to the output directory.")

args = parser.parse_args()

# Directory containing source files
source_dir = 'source'
# Directory to save modified files
output_dir = 'newout'

# Create output directory if it doesn't exist
os.makedirs(output_dir, exist_ok=True)

# Read class names from the file
with open('class_names.txt', 'r') as file:
    class_names = [line.strip() for line in file]

# Function to prepend 'juce::' to class names and save to a new file
def prepend_juce_to_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    changes_made = []
    full_content = ''.join(lines)  # Combine lines for processing in quiet mode

    for class_name in class_names:
        # Regex pattern to avoid adding 'juce::' if already preceded by any word and '::'
        pattern = fr"(?<![a-zA-Z_][\w]*::)\b{class_name}\b"
        new_content = re.sub(pattern, f"juce::{class_name}", full_content)

        # Check if any changes were made
        if new_content != full_content:
            changes_made.append(new_content)  # Store the modified content
            full_content = new_content  # Update full_content with the modified version

    # Write the modified content to a new file in the output directory
    if changes_made:
        # Preserve the directory structure
        relative_path = os.path.relpath(file_path, source_dir)
        output_file_path = os.path.join(output_dir, relative_path)
        os.makedirs(os.path.dirname(output_file_path), exist_ok=True)
        with open(output_file_path, 'w') as file:
            file.write(full_content)

        # Print changes if not in quiet mode
        if not args.quiet:
            for change in changes_made:
                print(f"Changes made in {output_file_path}:")
                for line_number, line in enumerate(change.splitlines(), start=1):
                    print(f"Line {line_number}: {line.strip()}")
    
    return len(changes_made)

# Iterate over all files in the source directory
total_files = 0
for root, dirs, files in os.walk(source_dir):
    for file_name in files:
        if file_name.endswith('.cpp') or file_name.endswith('.h'):
            total_files += 1

# Process files and show progress
processed_files = 0
for root, dirs, files in os.walk(source_dir):
    for file_name in files:
        if file_name.endswith('.cpp') or file_name.endswith('.h'):
            file_path = os.path.join(root, file_name)
            prepend_juce_to_file(file_path)
            processed_files += 1
            remaining_files = total_files - processed_files
            if not args.quiet:
                print(f"Processed {processed_files}/{total_files} ({remaining_files} files left): {file_path}")
            elif processed_files % 10 == 0:  # Print progress every 10 files in quiet mode
                print(f"Processed {processed_files}/{total_files} files...")

print("Prepending completed. Changes saved to the output directory.")

