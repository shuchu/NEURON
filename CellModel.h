/****************************************************************
 *
 *
 * CellModel --- data structure use to store large amount
 *                of Nueron cells
 *
 * shuchu.han@gmail.com 2012
****************************************************************/
#ifndef __CELLMODEL_H__
#define __CELLMODEL_H__

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "glut/glut.h"
#include "cell.h"


class CellModel
{
  public:
    typedef std::vector<Nueron*>::iterator Nueron_iterator;
    typedef std::vector<Synapse*>::iterator Synapse_iterator;

    CellModel(){};
    ~CellModel();

    Nueron_iterator nuerons_begin();
    Nueron_iterator nuerons_end();
    Synapse_iterator synapses_begin();
    Synapse_iterator synapses_end();

    /*void log(std::string& msg){m_log.append(msg);}
      void log(const std::string& msg){m_log.append(msg);}
      std::string& get_log(){return m_log;}
      void clear_log(){m_log.clear();}*/

    int size_of_nuerons();
    int num_of_types();

    bool load_data_from_file(std::string& file_name);

    //openGL functions;
    void draw_nuerons();
    void draw_synapses();


  private:
    std::vector<Nueron*> m_nuerons;
    std::vector<Synapse*> m_synapses;
    std::vector<char> m_types;
    /*std::string m_log;*/
};


CellModel::Nueron_iterator CellModel::nuerons_begin()
{
  return m_nuerons.begin();
}

CellModel::Nueron_iterator CellModel::nuerons_end()
{
  return m_nuerons.end();
}

CellModel::Synapse_iterator CellModel::synapses_begin()
{
  return m_synapses.begin();
}

CellModel::Synapse_iterator CellModel::synapses_end()
{
  return m_synapses.end();
}

CellModel::~CellModel()
{
  //release m_nuerons
  for(Nueron_iterator ni = this->nuerons_begin();
      ni != this->nuerons_end();
      ni++)
  {
    delete *ni;
  }

  //release m_synapses;
  for (Synapse_iterator si = this->synapses_begin();
      si != this->synapses_end();
      si++){
    delete *si;
  }

}

int CellModel::size_of_nuerons()
{
  return m_nuerons.size();
};

int CellModel::num_of_types()
{
  return m_types.size();
}

bool CellModel::load_data_from_file(std::string& file_name)
{
  //file stream 
  std::ifstream inputFile(file_name,std::ifstream::in);

  //check file stream status
  if(!inputFile.is_open()) {
    return false;
  } else {
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
      m_types[i] = type;
    }

    //load Nueron cells 
    getline(inputFile,line_buf);
    std::stringstream(line_buf) >> num_of_cells;

    for (int i = 0; i < num_of_cells; i++) {
      int type,x, y, z, a, d;

      // load Nueron
      getline(inputFile,line_buf);
      std::stringstream(line_buf) >> type >> x >> y >> z >> a >> d;
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
      for (int j = 0; j < d; j++) {
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
      if (line_buf.size() <= 1 )
        break;
      std::stringstream ss(line_buf);
      ss >>  s_type;

      Synapse* syn;

      if (s_type[0] == '#')
        continue;
      else if (s_type[0] == 'v') {
        // a VIA point  
        ss >>  from >> to >> viaX >> viaY >> viaZ >> x >> y >> z;
        syn = new Synapse(m_nuerons[from],m_nuerons[to],Point_3(viaX,viaY,viaZ),Point_3(x,y,z));
        syn->set_via_point(true);
      } else {
        // a normal Synapse
        std::stringstream(line_buf) >> from >> to >> x >> y >> z;
        syn = new Synapse(m_nuerons[from],m_nuerons[to],Point_3(x,y,z));
      }

      if (syn == NULL)
        continue;

      // add output Synapse to Neurons
      m_nuerons[from]->send_synapse(syn);
      m_nuerons[to]->rec_synapse(syn);
      m_synapses.push_back(syn);
    }  while (!inputFile.eof()) ;

    //debug
    std::cout << "loaded nuerons: " << m_nuerons.size() << std::endl;
    std::cout << "loaded synapses: " << m_synapses.size() << std::endl;

    inputFile.close();
  };

  return true;
};

void CellModel::draw_nuerons()
{  
  for (int i = 0; i < m_nuerons.size(); ++i){
    m_nuerons[i]->draw_soma();
  }
};

void CellModel::draw_synapses()
{
  for (int i = 0; i < m_synapses.size(); ++i){
    m_synapses[i]->draw();
  }
};

#endif
