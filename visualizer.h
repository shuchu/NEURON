#ifndef __VISUALIZER_H__
#define __VISUALIZER_H__

#include "QGLViewer/qglviewer.h"
#include "CellModel.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Viewer : public QGLViewer
{
  Q_OBJECT

  public:
    Viewer(QWidget* parent);
    ~Viewer() {
      delete [] m_frames;
    }

    void load_cellModel(CellModel* cm);
    void build_frames();
    void update_scene();
    void load_texture(); // load texture
    void update_aabb();
    void draw_corner_axis();
    void build_select_objects();
    int set_tex_id(int& type);

  protected:
    virtual void draw();
    //virtual void postDraw();
    //virtual void fastDraw();
    virtual void init();
    virtual QString helpString() const;
    void draw_box(int x, int y, int z, float scale);
    void draw_box(int lx, int ly, int lz, int rx, int ry, int rz);
    void update_magic_number();
    void update_scene_radius();

    public slots:
      void aabbBoxState(int state);
    void load_status();
    void show_P(int state);
    void show_N(int state);
    void show_G(int state);
    void show_B(int state);
    void show_A(int state);

    void show_S(int state);
    void show_T(int state);
    void show_I(int state);
    void show_C(int state);
    void show_M(int state);

    void show_R(int state);
    void show_D(int state);

    void show_den(int state); 
    void show_axon(int state);
    void show_syn(int state); //synapse
    void show_syn_via(bool state); //via point
    void show_syn_in(int state);
    void show_syn_out(int state);
    void show_ortho(bool state);
    void set_clip_x(int value);
    void set_clip_y(int value);
    void set_clip_z(int value);
    void enable_clip_plane(bool flag);
    void show_clip_plane(bool flag);
    void show_axis(bool flag);
    void set_rot_x(int value);
    void set_rot_y(int value);
    void set_rot_z(int value);
    void rotate_according_x();
    void rotate_according_y();
    void rotate_according_z();
    void show_selected(bool flag);

    // Selection functions
    virtual void drawWithNames();
    virtual void endSelection(const QPoint&);

    // Mouse events functions
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

  private: 
    ///////////////////////////////////////////
    //selection function
    void startManipulation();
    void drawSelectionRectangle() const;
    void addIdToSelection(int id);
    void removeIdFromSelection(int id);

    // Current rectangular selection
    QRect rectangle_;

    // Different selection modes
    enum SelectionMode { NONE, ADD, REMOVE };
    SelectionMode selectionMode_;
    QList<Nueron*> objects_;
    QList<int> selection_;
    bool m_selected;
    ///////////////////////////////////////////

    CellModel * m_cm;
    qglviewer::Frame *m_frames;
    GLuint m_texture[12];

    //axis
    bool m_axis;

    //rotate x
    double m_radius_x;
    double m_radius_y;
    double m_radius_z;

    //bounding box
    double aabb_low[3];
    double aabb_high[3];

    //orthogonal view
    bool m_ortho;

    //clip planes
    bool m_clip_show;
    double m_clip_x[4];
    double m_clip_y[4];
    double m_clip_z[4];

    //tags
    int m_aabb;
    int m_den;
    int m_axon;
    int m_syn;
    bool m_syn_via;
    int m_syn_in;
    int m_syn_out;

    //cell types;
    int m_magic_number;
    int m_P; // 000000000001
    int m_N; // 000000000010
    int m_G; // 000000000100
    int m_B; // 000000001000
    int m_A; // 000000010000

    int m_S; // 000000100000
    int m_T; // 000001000000
    int m_I; // 000010000000
    int m_C; // 000100000000
    int m_M; // 001000000000

    int m_R; // 010000000000
    int m_D; // 100000000000
};


#endif 
