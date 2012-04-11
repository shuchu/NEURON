/****************************************************************
 *  
 *
 *
 *  Cell.h  ---  definitaion of a Neuron Cell
 *
 *  shuchu.han@gmail.com
 *
 *  A CELL contains One soma, many dendrites and axons;
 *  
 *
****************************************************************/
#ifndef __CELL_H__
#define __CELL_H__

#include <vector>
#include "glut/glut.h"

#include "vector.h"
#include "drawShapes.h"

typedef vec3<int> Point_3;

class Nueron;
class Soma;

/* Dendrite cell */
class Dendrite
{
  public:
    Dendrite(Point_3& bl, Point_3& tr)
    {
      m_bl = bl;
      m_tr = tr;
    };
    ~Dendrite(){};

    //draw a rectangle by 6 quads;
    void draw()
    {
      drawCube(m_bl[0],m_bl[1],m_bl[2], 
          m_tr[0],m_tr[1],m_tr[2]);
    };

  private:
    Point_3 m_bl; //bottom left
    Point_3 m_tr; //top right
};

/* Axon cell*/
class Axon
{
  public:
    Axon(Point_3& bl, Point_3& tr)
    {
      m_bl = bl;
      m_tr = tr;
    };
    ~Axon(){};

    //draw a rectangle by 6 quads;
    void draw()
    {
      drawCube(m_bl[0],m_bl[1],m_bl[2], 
          m_tr[0],m_tr[1],m_tr[2]);
    }
  private:
    Point_3 m_bl;
    Point_3 m_tr;
};

/* Synapse connections */
class Synapse
{
  public:
    Synapse(Nueron* from, Nueron* to, Point_3& via, Point_3& pos)
    {
      m_from_nueron = from;
      m_to_nueron = to;
      m_pos = pos;
      m_via = via;
      m_rendered = false;
      m_via_point = true;
    };
    ~Synapse(){};

    Synapse(Nueron* from, Nueron* to,Point_3& pos)
    {
      m_from_nueron = from;
      m_to_nueron = to;
      m_pos = pos;
      //m_via = pos;
      m_rendered = false;
      m_via_point = false;
    };

    bool is_via_point()
    {
      return m_via_point;
    }

    bool is_rendered()
    {
      return m_rendered;
    }

    void set_rendered(bool status)
    {
      m_rendered = status;
    }

    void set_via_point(bool status)
    {
      m_via_point = status;  
    }

    Point_3 via_point()
    {
      if(m_via_point)
        return m_via;
      else 
        return m_pos;
    };

    Point_3 pos()
    {
      return m_pos;
    };

    //assume the order of lines are fixed.
	void draw(bool via_point);

    // enchance the effective
    void draw_enhanced()
    {
      drawCube(m_pos[0],m_pos[1],m_pos[2],1.0);
      if (m_via_point){
        drawCube(m_via[0],m_via[1],m_via[2],1.0);
      }
    };


  private:
    Nueron *m_from_nueron;
    Nueron *m_to_nueron;

    Point_3 m_pos;
    Point_3 m_via;
    bool m_rendered;
    bool m_via_point;
};


/* Soma cell */
class Soma
{
  public:
    Soma(Point_3& pos)
    {
      m_pos = pos;
    };
    ~Soma(){};

    void set_position(const Point_3& pos)
    {
      m_pos = pos;
    };

    Point_3& get_position()
    {
      return m_pos;
    }

    void draw()
    {
      drawCube(m_pos[0],m_pos[1],m_pos[2],4.0);
    }

  private:
    //// ouput and input synapses;
    //std::vector<Synapse*>    m_ouput;
    //std::vector<Synapse*>    m_input;

    // cell location
    Point_3 m_pos;
};


/* a single Nueron cell*/
class Nueron
{
  public:
    Nueron(){};
    ~Nueron(){
      delete m_soma;

      for (int i = 0; i < m_dendrites.size(); ++i)
        delete m_dendrites[i];

      for (int i = 0; i < m_axons.size(); ++i)
        delete m_axons[i];
    };

    Nueron(int& type, Point_3& pos, int& axon_num, int& den_num)
    {
      m_soma = new Soma(pos);

      m_type = type;
    };

    // receive a synapse signal
    void rec_synapse(Synapse* rec)
    {
      m_input.push_back(rec);
    }

    // send out a synapse signal
    void send_synapse(Synapse* send)
    {
      m_output.push_back(send);
    }

    // add one dendrite 
    void add_dendrite( Dendrite* den)
    {
      m_dendrites.push_back(den);
    }

    // add one axon
    void add_axon( Axon* axon)
    {
      m_axons.push_back(axon);
    }

    // set type of a cell
    void set_type(const int& type)
    {
      m_type = type;
    };

    int type()
    {
      return m_type;
    };

    void set_id(const int& id)
    {
      m_id = id;
    }

    int& id()
    {
      return m_id;
    };

    Soma* soma()
    {
      return m_soma;
    }

    //draw soma;
    void draw_soma()
    {
      m_soma->draw();
    };

    void draw_dendrites()
    {
      for(int i = 0; i < m_dendrites.size(); ++i)
        m_dendrites[i]->draw();
    }

    void draw_axons()
    {
      for (int i = 0; i < m_axons.size(); ++i)
        m_axons[i]->draw();
    }

	//draw the input synapses
	void draw_input_synapse(bool via_flag)
	{
		//one soma to another soma
		//may have via point
		for(int i = 0; i < m_input.size(); ++i)
		{
			m_input[i]->draw(via_flag);
		}
	};

	//draw the ouput synapses
	void draw_output_synapse(bool via_flag)
	{
		for (int i = 0; i < m_output.size(); ++i)
		{
			m_output[i]->draw(via_flag);
		}
	};

	Point_3& position()
	{
		return m_soma->get_position();
	}

  private:
    // ouput and input synapses;
    std::vector<Synapse*>    m_output;
    std::vector<Synapse*>    m_input;

    Soma* m_soma; //each neuron only has one soma;
    std::vector<Dendrite*> m_dendrites; 
    std::vector<Axon*> m_axons;

    int m_id;
    int m_type;
};


#endif
