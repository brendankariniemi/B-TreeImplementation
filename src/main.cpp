/**
 * @file main.cpp
 * @brief Main program file for processing CSV data related to ZIP code records using a B+ tree.
 *        This program supports adding, deleting, and searching records, as well as displaying
 *        various views of the data, such as extrema, sequence sets, and the tree structure itself.
 */

#include <iostream>
#include <iomanip>
#include "RecordBuffer.h"
#include "HeaderBuffer.h"
#include "Record.h"
#include "RecordFile.h"
#include "BTreeFile.h"

using namespace std;

// Function prototypes
bool processCommandLine(int argc, char* argv[], HeaderBuffer &headerBuffer, vector<vector<string>> &actions);
void addRecords(BTreeFile &bTreeFile, HeaderBuffer &headerBuffer, const string& fileName);
void deleteRecords(BTreeFile &bTreeFile, HeaderBuffer &headerBuffer, const string& fileName);
void searchIndex(BTreeFile &bTreeFile, vector<string> zipcodes);

/**
 * Main function which serves as the entry point for the program. It processes command line
 * arguments to perform various operations on ZIP code records using a B+ tree structure.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Program exit status.
 */
int main(int argc, char* argv[]) {
    HeaderBuffer headerBuffer; // Object to manage file header operations.

    string bTreeFileName; // Name of the B+ tree file.
    vector<vector<string>> actions; // Stores parsed command line actions.

    // Process command line arguments and exit if failed.
    if (!processCommandLine(argc, argv, headerBuffer, actions)) {
        return -1;
    } else {
        bTreeFileName = argv[argc - 1]; // Last argument is the B+ tree file name.
    }

    BTreeFile bTreeFile(headerBuffer, 10); // Initialize B+ tree with default order 10.
    // Attempt to open the B+ tree file and exit if failed.
    if (!bTreeFile.openFile(bTreeFileName)) {
        cout << "Failed to open " << bTreeFileName << "!" << endl;
        return -1;
    }

    // Iterate through actions derived from command line arguments and perform them.
    for (int i = 0; i < actions.size(); i++) {
        string action = actions[i][0]; // Action type (e.g., -ADD_RECORDS, -SEARCH).
        // Call specific function based on the action.
        if (action == "-ADD_RECORDS") {
            addRecords(bTreeFile, headerBuffer, actions[i][1]);
        } else if (action == "-DELETE_RECORDS") {
            deleteRecords(bTreeFile, headerBuffer, actions[i][1]);
        } else if (action == "-DISPLAY_EXTREMA") {
            bTreeFile.displayExtrema(cout, actions[i][1]);
        } else if (action == "-DISPLAY_SEQUENCE_SET") {
            bTreeFile.displaySequenceSet(cout);
        } else if (action == "-DUMP_TREE") {
            bTreeFile.displayTree(cout);
        } else if (action == "-SEARCH") {
            searchIndex(bTreeFile, actions[i]);
        }
    }

    return 0;
}


/**
 * Processes command line arguments to configure operations such as adding or deleting records,
 * and adjusting B+ tree properties. It parses the arguments to determine the actions to be taken,
 * configuring parameters like block size and the minimum block capacity, or scheduling tasks like
 * adding records, deleting records, displaying various data views, or searching within the B+ tree.
 *
 * @param argc The number of command line arguments.
 * @param argv The array containing the command line arguments.
 * @param headerBuffer A reference to the HeaderBuffer object for managing file header settings.
 * @param actions A reference to a vector that will store parsed actions and their parameters derived from the arguments.
 * @return A boolean indicating if the command line processing was successful.
 */
bool processCommandLine(int argc, char* argv[], HeaderBuffer &headerBuffer, vector<vector<string>> &actions) {
    // Ensure at least the filename is provided.
    if (argc < 2) {
        cout << "Error: Filename parameter is required." << endl;
        return false;
    }

    // Iterate through all arguments to configure settings or schedule tasks.
    for (int i = 1; i < argc - 1; i++) {
        string arg = argv[i]; // Current argument being processed.

        if (arg == "-BLOCK_SIZE") {
            if (i + 1 < argc) {
                headerBuffer.blockSize = stoi(argv[++i]); // Parse and set block size, advance to next argument.
            } else {
                cout << "Error: -BLOCK_SIZE flag requires a numerical value." << endl;
                return false;
            }
        } else if (arg == "-MINIMUM_BLOCK_CAPACITY") {
            if (i + 1 < argc) {
                headerBuffer.minimumBlockCapacity = stoi(argv[++i]); // Parse and set minimum block capacity, advance.
            } else {
                cout << "Error: -MINIMUM_BLOCK_CAPACITY flag requires a numerical value." << endl;
                return false;
            }
        } else if (arg == "-ADD_RECORDS") {
            if (i + 1 < argc) {
                actions.push_back({arg, argv[++i]}); // Schedule addition of records, move past filename.
            } else {
                cout << "Error: -ADD_RECORDS flag requires a filename." << endl;
                return false;
            }
        } else if (arg == "-DELETE_RECORDS") {
            if (i + 1 < argc) {
                actions.push_back({arg, argv[++i]}); // Schedule deletion of records, move past filename.
            } else {
                cout << "Error: -DELETE_RECORDS flag requires a filename." << endl;
                return false;
            }
        } else if (arg == "-DISPLAY_EXTREMA") {
            vector<string> tmp = {arg};
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                tmp.push_back(argv[++i]); // Add state argument if present, then advance.
            }
            actions.push_back(tmp);
        } else if (arg == "-DISPLAY_SEQUENCE_SET") {
            actions.push_back({arg}); // Schedule display of the sequence set.
        } else if (arg == "-DUMP_TREE") {
            actions.push_back({arg}); // Schedule display of the B+ tree structure.
        } else if (arg == "-SEARCH") {
            vector<string> tmp = {arg};
            // Accumulate all zip codes until another flag or the end of arguments.
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                tmp.push_back(argv[++i]);
            }
            actions.push_back(tmp);
        }
    }

    return true; // Command line arguments processed successfully.
}


/**
 * Adds records to the B+ tree from a specified file. This function opens the file containing new records,
 * reads each record into a RecordBuffer object, and then inserts each record into the B+ tree.
 * If the specified file contains records in a format not compatible with direct insertion into the B+ tree,
 * a pre-processing step might be necessary to convert the file into a suitable format.
 *
 * @param bTreeFile Reference to the BTreeFile object to perform operations on the B+ tree.
 * @param headerBuffer Reference to the HeaderBuffer object for managing file headers, used here
 *        for configuring the RecordFile object for reading the new records.
 * @param fileName Name of the file containing the new records to add. The file format is expected
 *        to be compatible with the RecordBuffer and RecordFile specifications for direct reading
 *        and insertion into the B+ tree.
 */
void addRecords(BTreeFile &bTreeFile, HeaderBuffer &headerBuffer, const string& fileName) {
    RecordBuffer recordBuffer; // Buffer for individual records to be added.
    RecordFile newRecordsFile(headerBuffer); // File handler for the new records, configured with the header buffer.

    string lengthIndicatedFile; // Filename for a temporary, length-indicated version of the records file.
    // Attempt to open the file containing new records. If unsuccessful, print an error message and exit this function.
    if (!newRecordsFile.openFile(fileName, lengthIndicatedFile)) {
        cout << "Failed to open " << fileName << " for adding records." << endl;
        return;
    }

    // Read each record from the file into the record buffer, then insert it into the B+ tree.
    while (newRecordsFile.read(recordBuffer) != -1) {
        // Insert the current record into the B+ tree.
        bTreeFile.insert(recordBuffer);
    }

    // Clean up: Attempt to delete the temporary file used for processing.
    if (std::remove(lengthIndicatedFile.c_str()) != 0) {
        cout << "Failed to delete temporary file: " << lengthIndicatedFile << endl;
    }
}


/**
 * Deletes records from the B+ tree based on the contents of a specified file.
 * This function iterates through each record in the file, constructs a RecordBuffer object for it,
 * and then attempts to remove it from the B+ tree.
 *
 * @param bTreeFile Reference to the BTreeFile object to operate on the B+ tree.
 * @param headerBuffer Reference to the HeaderBuffer object for managing file headers.
 * @param fileName Name of the file containing records to delete.
 */
void deleteRecords(BTreeFile &bTreeFile, HeaderBuffer &headerBuffer, const string& fileName) {
    RecordBuffer recordBuffer; // Buffer for individual records to be deleted.
    RecordFile deleteRecordsFile(headerBuffer); // Handles file operations for records to delete.

    string lengthIndicatedFile; // Temporary file for length-indicated processing.
    if (!deleteRecordsFile.openFile(fileName, lengthIndicatedFile)) {
        cout << "Failed to open " << fileName << " for deletion!" << endl;
        return;
    }

    // Loop to read each record from the file and attempt to delete from the B+ tree.
    while (deleteRecordsFile.read(recordBuffer) != -1) {
        bTreeFile.remove(recordBuffer);
    }

    // Clean up: Attempt to delete the temporary file used for processing.
    if (std::remove(lengthIndicatedFile.c_str()) != 0) {
        cout << "Failed to delete temporary file: " << lengthIndicatedFile << endl;
    }
}


/**
 * Searches the B+ tree for records matching the specified ZIP codes and displays them.
 * Each ZIP code provided in the zipcodes vector is converted to an integer and searched
 * within the B+ tree. If a record is found, it is displayed; otherwise, a not found message is shown.
 *
 * @param bTreeFile Reference to the BTreeFile object for B+ tree operations.
 * @param zipcodes A vector of strings representing the ZIP codes to search for.
 */
void searchIndex(BTreeFile &bTreeFile, vector<string> zipcodes) {
    RecordBuffer recordBuffer; // Buffer to hold the search result record.

    // Skip the first element (action command) and start from the first ZIP code.
    for (int i = 1; i < zipcodes.size(); i++) {
        int zipCode = stoi(zipcodes[i]); // Convert ZIP code string to integer.
        if (bTreeFile.search(recordBuffer, zipCode) != -1) {
            // If found, convert the record buffer back to a Record object and display.
            Record record = Record(recordBuffer);
            record.display();
        } else {
            // If not found, display a message indicating the ZIP code was not found.
            cout << "ZIP Code " << zipcodes[i] << " not found in the B+ tree." << endl;
        }
    }
}
