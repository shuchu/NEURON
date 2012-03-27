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
#include <fstream>
#include <climits>

#include "glut/glut.h"
#include "cell.h"


class CellModel
{
  public:
    typedef std::vector<Nueron*>::iterator Nueron_iterator;
    typedef std::vector<Synapse*>::iterator Synapse_iterator;

    CellModel()
	{
		////bounding box
		//bbox_bl = Point_3(INT_MIN,INT_MIN,INT_MIN);
		//bbox_tr = Point_3(INT_MAX,INT_MAX,INT_MAX);
	};
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
	void compute_bounding_box();
	Point_3& bbox_bottom_left();
	Point_3& bbox_top_right();

    //openGL functions;
    void draw_nuerons();
    void draw_synapses();
	void draw_AABB();
	
  private:
    std::vector<Nueron*> m_nuerons;
    std::vector<Synapse*> m_synapses;
    std::vector<char> m_types;
    /*std::string m_log;*/

	Point_3 bbox_bl;
	Point_3 bbox_tr;
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

Point_3& CellModel::bbox_bottom_left()
{
	return bbox_bl;
};

Point_3& CellModel::bbox_top_right()
{
	return bbox_tr;
};

void CellModel::compute_bounding_box()
{
	bbox_bl = Point_3(INT_MAX,INT_MAX,INT_MAX);
	bbox_tr = Point_3(INT_MIN,INT_MIN,INT_MIN);

	//just check soma
	for (int i = 0; i < m_nuerons.size(); ++i)
	{
		Point_3 soma = m_nuerons[i]->soma()->get_position();
		for (int j = 0; j < 3; ++j){
			if (soma[j] < bbox_bl[j])
				bbox_bl[j] = soma[j];
			if (soma[j] > bbox_tr[j])
				bbox_tr[j] = soma[j];
		};
	};

	//debug
	std::cout << "botom left: " << bbox_bl[0] << " "
                                << bbox_bl[1] << " "
                                << bbox_bl[2] <<std::endl;
	std::cout << "top right: " << bbox_tr[0] << " "
                               << bbox_tr[1] << " "
                               << bbox_tr[2] << std::endl;
};

void CellModel::draw_AABB()
{
	drawCube(bbox_bl[0],bbox_bl[1],bbox_bl[2],\
		           bbox_tr[0],bbox_tr[1],bbox_tr[2]);
	/*glBegin(GL_LINE_LOOP);
	glVertex3i(bbox_bl[0],bbox_bl[1],bbox_bl[2]);
	glVertex3i(bbox_tr[0],bbox_bl[1],bbox_bl[2]);
	glVertex3i(bbox_tr[0],bbox_bl[1],bbox_tr[2]);
	glVertex3i(bbox_bl[0],bbox_bl[1],bbox_tr[2]);
    glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3i(bbox_bl[0],bbox_tr[1],bbox_bl[2]);
	glVertex3i(bbox_tr[0],bbox_tr[1],bbox_bl[2]);
	glVertex3i(bbox_tr[0],bbox_tr[1],bbox_tr[2]);
	glVertex3i(bbox_bl[0],bbox_tr[1],bbox_tr[2]);
    glEnd();
	
	glBegin(GL_LINES);
	glVertex3i(bbox_bl[0],bbox_bl[1],bbox_bl[2]);
	glVertex3i(bbox_bl[0],bbox_tr[1],bbox_bl[2]);
	
    glVertex3i(bbox_tr[0],bbox_bl[1],bbox_bl[2]);
	glVertex3i(bbox_tr[0],bbox_tr[1],bbox_bl[2]);
    
    glVertex3i(bbox_tr[0],bbox_bl[1],bbox_tr[2]);
	glVertex3i(bbox_tr[0],bbox_tr[1],bbox_tr[2]);
	
    glVertex3i(bbox_bl[0],bbox_bl[1],bbox_tr[2]);
	glVertex3i(bbox_bl[0],bbox_tr[1],bbox_tr[2]);
    glEnd();*/
};


#endif
