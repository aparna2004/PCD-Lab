#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>

// Enum for data types
enum class DataType {
    INT,
    FLOAT,
    DOUBLE,
    CHAR,
    STRING,
    VOID,
    UNKNOWN
};

// Structure to represent a symbol
struct Symbol {
    std::string name;
    DataType type;
    std::string scope;

    Symbol(const std::string& name, DataType type, const std::string& scope)
        : name(name), type(type), scope(scope) {}
};

// Class to represent a symbol table
class SymbolTable {
private:
    std::unordered_map<std::string, Symbol> table;

public:
    // Insert a new symbol into the symbol table
    void insert(const std::string& name, DataType type, const std::string& scope) {
        std::string key = name + ":" + scope; // Unique key for each scope
        table.emplace(key, Symbol(name, type, scope));
    }

    // Find a symbol in the symbol table
    bool find(const std::string& name, const std::string& scope) const {
        std::string key = name + ":" + scope;
        return table.find(key) != table.end();
    }

    // Get symbol details
    Symbol getSymbol(const std::string& name, const std::string& scope) const {
        std::string key = name + ":" + scope;
        if (find(name, scope)) {
            return table.at(key);
        }
        throw std::runtime_error("Symbol not found");
    }

    // Display all symbols in the table
    void display() const {
        for (const auto& entry : table) {
            const Symbol& symbol = entry.second;
            std::cout << "Name: " << symbol.name
                      << ", Type: " << static_cast<int>(symbol.type)
                      << ", Scope: " << symbol.scope << std::endl;
        }
    }
};

// Function to convert string to DataType
DataType stringToDataType(const std::string& type) {
    if (type == "int") return DataType::INT;
    if (type == "float") return DataType::FLOAT;
    if (type == "double") return DataType::DOUBLE;
    if (type == "char") return DataType::CHAR;
    if (type == "void") return DataType::VOID;
    return DataType::UNKNOWN;
}

// Function to analyze code from a file
void analyzeCodeFromFile(const std::string& filename, SymbolTable& symbolTable) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string currentScope = "global"; // Start with global scope

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string type;
        std::string name;

        // Detect function definitions to create a new scope
        if (line.find("void") != std::string::npos ||
            line.find("int") != std::string::npos ||
            line.find("float") != std::string::npos ||
            line.find("double") != std::string::npos ||
            line.find("char") != std::string::npos) {
            lineStream >> type >> name; // Read type and name
            if (lineStream.fail() || name.empty()) {
                continue; // Skip invalid lines
            }

            // Remove any trailing parentheses or semicolon
            if (name.back() == '(') {
                currentScope = name; // Set the current scope as the function name
                continue; // Skip function declaration
            }
            if (name.back() == ';') {
                name.pop_back();
            }

            // Skip unwanted symbols like <string>
            if (name == "string") continue;

            // Insert into symbol table only if it's a valid symbol
            symbolTable.insert(name, stringToDataType(type), currentScope);
        } else if (line.find("}") != std::string::npos) {
            // End of function scope, revert to global
            currentScope = "global";
        }
    }

    file.close();
}

int main() {
    SymbolTable symbolTable;

    // File containing C++ code
    std::string filename = "code.cpp"; // Adjust this to your file's path

    // Analyze the code from the file
    analyzeCodeFromFile(filename, symbolTable);

    // Display all symbols
    std::cout << "Symbol Table:\n";
    symbolTable.display();

    // Example of finding a symbol
    std::string symbolName = "b"; // Change this to test different symbols
    if (symbolTable.find(symbolName, "global")) {
        Symbol symbol = symbolTable.getSymbol(symbolName, "global");
        std::cout << "Found Symbol: " << symbol.name
                  << ", Type: " << static_cast<int>(symbol.type)
                  << ", Scope: " << symbol.scope << std::endl;
    } else {
        std::cout << "Symbol '" << symbolName << "' not found in global scope." << std::endl;
    }

    return 0;
}