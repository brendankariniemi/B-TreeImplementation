### B+ Tree

#### Project Overview
This B+ Tree Project offers a utility for managing state-related ZIP Code records using a B+ Tree data structure. The software allows for efficient data processing with functionalities to add, delete, search, and display records stored in a B+ Tree, making it a powerful tool for data management tasks.

#### Features
- **Add Records:** Dynamically insert ZIP Code records from a file.
- **Delete Records:** Remove records from the B+ Tree.
- **Search Records:** Search and retrieve specific ZIP Code information.
- **Display Data:** Show the B+ Tree structure, sequence set, or the extremal records for specific states.
- **File Handling:** Automatically handles file creation and operations if the specified B+ Tree file does not exist.

#### System Requirements
- C++ Compiler (g++ recommended)
- Standard C++ Libraries
- Linux or Unix-like environment for best performance

#### Installation and Setup
1. **Download the Source Code:** Clone or download the ZIP file from the repository.
2. **Compiling the Program:**
   - Navigate to the source code directory.
   - Compile using the cmake file
   
#### Usage
Run the program using the following command format:
```bash
./zipcode [OPTION] [ADDITIONAL PARAMETERS]
```

Options include:
- `-ADD_RECORDS [filename]`: Adds records from the specified file.
- `-DELETE_RECORDS [filename]`: Deletes records as per the file.
- `-DISPLAY_EXTREMA [state or "*"]`: Displays the extremal records for a specific state or all states.
- `-DISPLAY_SEQUENCE_SET`: Displays all records in the sequence set.
- `-DUMP_TREE`: Outputs the structure of the B+ Tree.
- `-SEARCH [zipcode1] [zipcode2]`: Searches for records between two ZIP codes.

Examples:
```bash
./zipcode -SEARCH 10001 20001
./zipcode -ADD_RECORDS records_to_add.txt
```
