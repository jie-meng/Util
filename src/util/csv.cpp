#include "csv.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <istream>
#include "string.hpp"

using namespace std;

namespace util
{

typedef UtilSharedPtr< vector<string> > SpStrVec;

//from http://www.zedwood.com/article/cpp-csv-parser
void csv_read_row(std::istream &in, char delimiter, std::vector<std::string>* pout_strvec);
void csv_read_row(std::string &in, char delimiter, std::vector<std::string>* pout_strvec);

const char kDelimeter = ',';

struct Csv::CsvImpl
{
    CsvImpl()
    {}

    CsvImpl(const std::string& file)
    { read(file); }

    bool read(const std::string& file)
    {
        matrix_.clear();

        std::ifstream ifs(file.c_str());
        if (ifs.is_open())
        {
            std::string line;
            while(!ifs.eof())
            {
                std::getline(ifs, line);

                SpStrVec spstrvec(new std::vector<std::string>());
                csv_read_row(line, kDelimeter, spstrvec.get());
                if (getTotalRows() != 0 && spstrvec->size() != getTotalCols())
                {
                    matrix_.clear();
                    return false;
                }

                matrix_.push_back(spstrvec);
            }
            ifs.close();
        }

        return true;
    }

    bool write(const std::string& file)
    {
        std::ofstream ofs;
        ofs.open(file.c_str());
        if (ofs.is_open())
        {
            vector<SpStrVec>::iterator it;
            for (it = matrix_.begin(); it != matrix_.end(); ++it)
            {

                ofs<<"";
            }
            ofs.close();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool write()
    {
        return false;
    }

    size_t getTotalRows() const
    {
        return matrix_.size();
    }

    size_t getTotalCols() const
    {
        if (matrix_.empty())
            return 0;

        return matrix_[0]->size();
    }

    vector<SpStrVec> matrix_;
};

Csv::Csv() :
    pimpl_(new CsvImpl())
{
    //ctor
}

Csv::Csv(const std::string& file) :
    pimpl_(new CsvImpl(file))
{

}

Csv::~Csv()
{
    //dtor
}

bool Csv::read(const std::string& file)
{
    return pimpl_->read(file);
}

bool Csv::write(const std::string& file)
{
    return pimpl_->write(file);
}

bool Csv::write()
{
    return pimpl_->write();
}

size_t Csv::getTotalRows() const
{
    return pimpl_->getTotalRows();
}

size_t Csv::getTotalCols() const
{
    return pimpl_->getTotalCols();
}

void csv_read_row(std::string &line, char delimiter, std::vector<std::string>* pout_strvec)
{
    if (!pout_strvec)
        return;

    std::stringstream ss(line);
    csv_read_row(ss, delimiter, pout_strvec);
}

void csv_read_row(std::istream &in, char delimiter, std::vector<std::string>* pout_strvec)
{
    std::stringstream ss;
    bool inquotes = false;
    //std::vector<std::string> row;//relying on RVO
    while(in.good())
    {
        char c = in.get();
        if (!inquotes && c=='"') //beginquotechar
        {
            inquotes=true;
        }
        else if (inquotes && c=='"') //quotechar
        {
            if ( in.peek() == '"')//2 consecutive quotes resolve to 1
            {
                ss << (char)in.get();
            }
            else //endquotechar
            {
                inquotes=false;
            }
        }
        else if (!inquotes && c==delimiter) //end of field
        {
            pout_strvec->push_back( ss.str() );
            ss.str("");
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            if(in.peek()=='\n') { in.get(); }
            pout_strvec->push_back( ss.str() );
            //return row;
        }
        else
        {
            ss << c;
        }
    }
}

} //namespace util
