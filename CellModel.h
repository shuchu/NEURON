/****************************************************************
 *
 *
 * CellModel --- data structure use to store large amount
 *                of Nueron cells
 *
 * shuchu.han@gmail.com 2012
****************************************************************/

#include <vector>
#include <string>
#include <sstream>

#include "cell.h"


class CellModel
{
  public:
    CellModel();
    ~CellModel();

    int size_of_nuerons();
    int num_of_types();

    bool load_data_from_file(const string& file_name)

  private:
    std::vector<Neuron*> m_nuerons;
    std::vector<char> m_types;

};

int CellModel::size_of_nuerons()
{
  return m_nuerons.size();
};

int CellModel::num_of_types()
{
  return m_types.size();
}

bool CellModel::load_data_from_file(const string& file_name)
{
  //file stream 
  std::ifstream inputFile(file_name,std::ifstream::in);

  //check file stream status
  if(!inputFile.is_open()) {
    return false;
  } else {
    std::vector<char> cell_types;   // cell type ---- ID
    int num_of_types; //num of cell types
    int num_of_cells;

    // load number of types
    std::string line_buf;
    getline(inputFile,line_buf);
    std::stringstream(line_buf) >> num_of_types;

    //create types
    m_types.resize(num_of_types);
    for (int i = 0; i < num_of_types; ++i) {
      char type;
      getline(inputFile,line_buf);
      std::stringstream(line_buf) >> type;
      cell_types[i] = type;
    }

    //load Nueron cells 
    getline(inputFile,line_buf);
    std::stringstream(line_buf) >> num_of_cells;

    for (int i = 0; i < num_of_cells; i++) {
      int type,x, y, z, a, d;

      // load Nueron
      inputFile >> type >> x >> y >> z >> a >> d;
      Nueron* nueron_ptr = new Nueron(type,Point_3(x,y,z),a,d);
      nueron_ptr->set_id(i);

      // load Axon
      int x_begin, x_end, y_begin, y_end, z_begin, z_end;
      for (int j = 0; j < a; j++) {
        getline(inputFile,line_buf);
        std::stringstream(line_buf) >> x_begin >> x_end >> y_begin \
                                    >> y_end >> z_begin >> z_end;
        Axon* axon = new Axon(Point_3(x_begin,y_begin,z_begin),
                              Point_3(x_end,y_end,z_end));
        nueron_ptr->add_axon(axon);
      }

      // load dendrite
      for (intj = 0; j < d; j++) {
        getline(inputFile,line_buf);
        std::stringstream(line_buf) >> x_begin >> x_end >> y_begin \
                                    >> y_end >> z_begin >> z_end;
        Dendrite* den = new Dendrite(Point_3(x_begin,y_begin,z_begin),
                                     Point_3(x_end,y_end,z_end));
        nueron_ptr->add_dendrite(den);
      }

      //store the data;
      this->m_nuerons.push_back(nueron_ptr);
    } // for cells

    // load synapse
    do{
      int from(-1), to, x, y, z, viaX, viaY, viaZ;
      std::string s_type; //type of snapse

      getline(inputFile,line_buf);
      std::stringstream ss(line_buf);
      ss >>  s_type;

      Synapse* syn;

      if (s_type[0] == "#")
        continue;
      else if (s_type[0] == 'v') {
        // a VIA point  
        ss >>  from >> to >> x >> y >> z;
        syn = new Synapse(from,to,Point_3(x,y,z));
        syn->set_via_point(true);
      } else {
        // a normal Synapse
        std::stringstream(line_buf) >> from >> to >> x >> y >> z;
        syn = new Synapse(from,to,Point_3(x,y,z));
      }
      
        if (syn == NULL)
          continue;
        // add output Synapse to Neurons
        m_nuerons[from]->send_synapse(syn);
        m_nuerons[to]->rec_synapse(syn);
      }

    }  while (!inputFile.eof()) ;

    inputFile.close();
  }
};

