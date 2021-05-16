#include <csv.hpp>

Csv::Csv() : Csv("out", "file") { }

Csv::Csv(std::string dir, std::string name) {
    // delimiter and newline definitions
    delim = ",";
    nl = "\n";
    fol = true;

    // create directory if it does not exist
    if (!std::filesystem::exists(dir))
        std::filesystem::create_directories(dir);

    // check if we would override a file, do not
    int i = 0;
    std::string suffix = "";
    path = dir + "/" + name + suffix + ".csv";

    while (std::filesystem::exists(path)) {
        suffix = "_" + std::to_string(i);
        path = dir + "/" + name + suffix + ".csv";
        i++;
    }

    csv.open(path);
}

Csv::Csv(std::string dir, std::string name, std::vector<std::string> head) : Csv(path, name) {
    // add header to file
    for (int i = 0; i < head.size() - 1; i++)
        csv << head[i] << delim;

    csv << head.back() << nl;
}

Csv::~Csv() {
    csv.close();
}

Csv& Csv::operator<<(const std::string value) {
    if (fol) {
        fol = false;
    } else {
        csv << delim;
    }

    csv << value;

    return *this;
}

Csv& Csv::operator<<(char value) {
    csv << value;

    if (fol) {
        fol = false;
    } else {
        csv << delim;
    }

    return *this;
}

Csv& Csv::operator<<(const char *value) {
    if (fol) {
        fol = false;
    } else {
        csv << delim;
    }

    csv << value;

    return *this;
}

Csv& Csv::operator<<(bool value) {
    if (fol) {
        fol = false;
    } else {
        csv << delim;
    }

    csv << value;

    return *this;
}

Csv& Csv::operator<<(int value) {
    if (fol) {
        fol = false;
    } else {
        csv << delim;
    }

    csv << value;

    return *this;
}

Csv& Csv::operator<<(double value) {
    if (fol) {
        fol = false;
    } else {
        csv << delim;
    }

    csv << value;

    return *this;
}

typedef std::ostream &(*io_manip_ptr_t)(std::ostream &);

Csv& Csv::operator<<(io_manip_ptr_t fct) {
    csv << fct;

    if (fct == (io_manip_ptr_t) &std::endl)
        fol = true;

    return *this;
}

std::string Csv::to_string() {
    std::stringstream os;

    os << "Csv{delim: " << delim
    << ", nl: " << nl
    << ", path: " << path << "}";

    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Csv& pl) {
    os << "Csv{delim: " << pl.delim
    << ", nl: " << pl.nl
    << ", path: " << pl.path << "}";
    return os;
}