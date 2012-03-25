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

#include <vector.h>
#include <GL/gl.h>

typedef vec3<int> Point_3;

/* Dendrite cell */
class Dendrite
{
public:
  Dendrite(Point_3& bl, Point_3& tr)
  {
    m_bl = bl;
    m_tr = tr;
  };
  ~Dendrite();

private:
  Point_3 m_bl; //bottom left
  Point_3 m_tr; //top right
};

/* Axon cell*/
class Axon
{
  public:
  Axon(Point_3& bl, Point_3& tr)
  {i
    m_bl = bl;
    m_tr = tr;
  };
  ~Axon();

private:
  Point_3 m_bl;
  Point_3 m_tr;
};

/* Synapse connections */
class Synapse
{
  public:
    Synapse(int& from, int& to, Point_3& pos)
    {
      m_from = from;
      m_to = to;
      m_pos = pos;
      m_rendered = false;
      m_via_point = false;
    };
    ~Synapse();

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

  private:
    int m_from;
    int m_to;
    Point_3 m_pos;
    bool m_rendered;
    bool m_via_point;
};


/* Soma cell */
class Soma
{
  public:
    Soma(Point& pos)
    {
      m_pos = pos;
    };
    ~Soma();

    void set_position(const Point_3& pos)
    {
      m_pos = pos;
    };

    Point_3& get_position()
    {
      return m_post;
    }

  private:
    // ouput and input synapses;
    std::vector<Synapse*>    m_ouput;
    std::vector<Synapse*>    m_input;

    // cell location
    Point_3 m_pos;
};


/* a single Nueron cell*/
class Nueron
{
  public:
    Nueron();
    ~Nueron(){
      delete m_soma;
    };

    Nueron(int& type, Point_3& pos, int& axon_num, int& den_num)
    {
      m_soma = new Soma(pos);

      m_type = type;

      if (axon_num > 0)
        m_axons.resize(axon_num);

      if (den_num > 0)
        m_dendrites.resize(den_num);
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
    void add_dendrite(const Dendrite* den)
    {
      m_dendrites.push_back(den);
    }

    // add one axon
    void add_axon(const Axon* axon)
    {
      m_axons.push_back(axon);
    }

    // set type of a cell
    void set_type(const int& type)
    {
      m_type = type;
    };

    void set_id(const int& id)
    {
      m_id = id;
    }

    int& id()
    {
      return m_id;
    }

  private:
    // ouput and input synapses;
    std::vector<Synapse*>    m_ouput;
    std::vector<Synapse*>    m_input;
    
    Soma* m_soma; //each neuron only has one soma;
    std::vector<Dendrite*> m_dendrites; 
    std::vector<Axon*> m_axons;

    int m_id;
    int m_type;
};




