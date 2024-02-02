

#define CONTROLS 3

#define GUI_ELEMENTS 0

#define TAB_ELEMENTS 0


#define PLUGIN_UI_URI "http://github.com/mikeoliphant/neural-amp-modeler-lv2_ui"


#include "lv2_plugin.h"

#ifdef USE_ATOM

#define XLV2__neural_model "http://github.com/mikeoliphant/neural-amp-modeler-lv2#model"
#define OBJ_BUF_SIZE 1024


typedef struct {
    LV2_URID neural_model;
    LV2_URID atom_Object;
    LV2_URID atom_Int;
    LV2_URID atom_Float;
    LV2_URID atom_Bool;
    LV2_URID atom_Vector;
    LV2_URID atom_Path;
    LV2_URID atom_String;
    LV2_URID atom_URID;
    LV2_URID atom_eventTransfer;
    LV2_URID patch_Put;
    LV2_URID patch_Get;
    LV2_URID patch_Set;
    LV2_URID patch_property;
    LV2_URID patch_value;
} X11LV2URIs;

typedef struct {
    LV2_Atom_Forge forge;
    X11LV2URIs   uris;
    char *filename;
    char *dir_name;
} X11_UI_Private_t;

static inline void map_x11ui_uris(LV2_URID_Map* map, X11LV2URIs* uris) {
    uris->neural_model = map->map(map->handle, XLV2__neural_model);
    uris->atom_Object = map->map(map->handle, LV2_ATOM__Object);
    uris->atom_Int = map->map(map->handle, LV2_ATOM__Int);
    uris->atom_Float = map->map(map->handle, LV2_ATOM__Float);
    uris->atom_Bool = map->map(map->handle, LV2_ATOM__Bool);
    uris->atom_Vector = map->map(map->handle, LV2_ATOM__Vector);
    uris->atom_Path = map->map(map->handle, LV2_ATOM__Path);
    uris->atom_String = map->map(map->handle, LV2_ATOM__String);
    uris->atom_URID = map->map(map->handle, LV2_ATOM__URID);
    uris->atom_eventTransfer = map->map(map->handle, LV2_ATOM__eventTransfer);
    uris->patch_Put = map->map(map->handle, LV2_PATCH__Put);
    uris->patch_Get = map->map(map->handle, LV2_PATCH__Get);
    uris->patch_Set = map->map(map->handle, LV2_PATCH__Set);
    uris->patch_property = map->map(map->handle, LV2_PATCH__property);
    uris->patch_value = map->map(map->handle, LV2_PATCH__value);
}
#endif


#include "xresources.h"

void set_costum_theme(Widget_t *w) {
    w->color_scheme->normal = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.850, 0.850, 0.850, 1.000},
        .bg =       { 0.100, 0.100, 0.100, 1.000},
        .base =     { 0.000, 0.000, 0.000, 1.000},
        .text =     { 0.682, 0.686, 0.686, 1.000},
        .shadow =   { 0.000, 0.000, 0.000, 0.200},
        .frame =    { 0.000, 0.000, 0.000, 1.000},
        .light =    { 0.100, 0.100, 0.100, 1.000}
    };

    w->color_scheme->prelight = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 1.000, 0.000, 1.000, 1.000},
        .bg =       { 0.250, 0.250, 0.250, 1.000},
        .base =     { 0.300, 0.300, 0.300, 1.000},
        .text =     { 1.000, 1.000, 1.000, 1.000},
        .shadow =   { 0.100, 0.100, 0.100, 0.400},
        .frame =    { 0.300, 0.300, 0.300, 1.000},
        .light =    { 0.300, 0.300, 0.300, 1.000}
    };

    w->color_scheme->selected = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.900, 0.900, 0.900, 1.000},
        .bg =       { 0.200, 0.200, 0.200, 1.000},
        .base =     { 0.500, 0.180, 0.180, 1.000},
        .text =     { 1.000, 1.000, 1.000, 1.000},
        .shadow =   { 0.800, 0.180, 0.180, 0.200},
        .frame =    { 0.500, 0.180, 0.180, 1.000},
        .light =    { 0.500, 0.180, 0.180, 1.000}
    };

    w->color_scheme->active = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.000, 1.000, 1.000, 1.000},
        .bg =       { 0.000, 0.000, 0.000, 1.000},
        .base =     { 0.180, 0.380, 0.380, 1.000},
        .text =     { 0.750, 0.750, 0.750, 1.000},
        .shadow =   { 0.180, 0.380, 0.380, 0.500},
        .frame =    { 0.180, 0.380, 0.380, 1.000},
        .light =    { 0.180, 0.380, 0.380, 1.000}
    };

    w->color_scheme->insensitive = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.850, 0.850, 0.850, 0.500},
        .bg =       { 0.100, 0.100, 0.100, 0.500},
        .base =     { 0.000, 0.000, 0.000, 0.500},
        .text =     { 0.900, 0.900, 0.900, 0.500},
        .shadow =   { 0.000, 0.000, 0.000, 0.100},
        .frame =    { 0.000, 0.000, 0.000, 0.500},
        .light =    { 0.100, 0.100, 0.100, 0.500}
    };
}

#include "lv2_plugin.cc"


#ifdef USE_ATOM

static void notify_dsp(X11_UI *ui) {
    X11_UI_Private_t *ps = (X11_UI_Private_t*)ui->private_ptr;
    const X11LV2URIs* uris = &ps->uris;
    uint8_t obj_buf[OBJ_BUF_SIZE];
    lv2_atom_forge_set_buffer(&ps->forge, obj_buf, OBJ_BUF_SIZE);
    LV2_Atom_Forge_Frame frame;
    LV2_Atom* msg = (LV2_Atom*)lv2_atom_forge_object(&ps->forge, &frame, 0, uris->patch_Get);
    lv2_atom_forge_key(&ps->forge, uris->patch_property);

    ui->write_function(ui->controller, 0, lv2_atom_total_size(msg),
                       ps->uris.atom_eventTransfer, msg);
}

static inline LV2_Atom* write_set_file(LV2_Atom_Forge* forge, const LV2_URID control,
                        const X11LV2URIs* uris, const char* filename) {
    LV2_Atom_Forge_Frame frame;
    LV2_Atom* set = (LV2_Atom*)lv2_atom_forge_object(
                        forge, &frame, 1, uris->patch_Set);
    lv2_atom_forge_key(forge, uris->patch_property);
    lv2_atom_forge_urid(forge, control);
    lv2_atom_forge_key(forge, uris->patch_value);
    lv2_atom_forge_path(forge, filename, strlen(filename));
    lv2_atom_forge_pop(forge, &frame);
    return set;
}

static void file_load_response(void *w_, void* user_data) {
    Widget_t *w = (Widget_t*)w_;
    Widget_t *p = (Widget_t*)w->parent;
    X11_UI *ui = (X11_UI*) p->parent_struct;
    X11_UI_Private_t *ps = (X11_UI_Private_t*)ui->private_ptr;
    const LV2_URID urid = *(const LV2_URID*)w->parent_struct;
    if(user_data !=NULL) {
        free(ps->filename);
        ps->filename = NULL;
        ps->filename = strdup(*(const char**)user_data);
        uint8_t obj_buf[OBJ_BUF_SIZE];
        lv2_atom_forge_set_buffer(&ps->forge, obj_buf, OBJ_BUF_SIZE);
        LV2_Atom* msg = (LV2_Atom*)write_set_file(&ps->forge, urid, &ps->uris, ps->filename);
        ui->write_function(ui->controller, 0, lv2_atom_total_size(msg),
                           ps->uris.atom_eventTransfer, msg);
        free(ps->filename);
        ps->filename = NULL;
        ps->filename = strdup("None");
        ui->loop_counter = 6;
    }
}

static void dnd_load_response(void *w_, void* user_data) {
    if(user_data !=NULL) {
        Widget_t *w = (Widget_t*)w_;
        Widget_t *c = w->childlist->childs[0];
        char* dndfile = NULL;
        dndfile = strtok(*(char**)user_data, "\r\n");
        while (dndfile != NULL) {
            if (strstr(dndfile, ".nam")) {
                file_load_response((void*)c, (void*)&dndfile);
                break;
            }
            dndfile = strtok(NULL, "\r\n");
        }
    }
}

void send_controller_message(Widget_t *w, const LV2_URID control) {
    Widget_t *p = (Widget_t*)w->parent;
    X11_UI *ui = (X11_UI*) p->parent_struct;
    X11_UI_Private_t *ps = (X11_UI_Private_t*)ui->private_ptr;
    const X11LV2URIs* uris = &ps->uris;
    const float value = adj_get_value(w->adj);
    uint8_t obj_buf[OBJ_BUF_SIZE];
    lv2_atom_forge_set_buffer(&ps->forge, obj_buf, OBJ_BUF_SIZE);
    LV2_Atom_Forge_Frame frame;
    LV2_Atom* msg = (LV2_Atom*)lv2_atom_forge_object(&ps->forge, &frame, 0, uris->patch_Set);
    lv2_atom_forge_key(&ps->forge, uris->patch_property);
    lv2_atom_forge_urid(&ps->forge, control);
    lv2_atom_forge_key(&ps->forge, uris->patch_value);
    switch(w->data) {
        case -2:
             lv2_atom_forge_int(&ps->forge, (int)value);
        break;
        case -3:
             lv2_atom_forge_bool(&ps->forge, (int)value);
        break;
        default:
            lv2_atom_forge_float(&ps->forge, value);
        break;
    }
    lv2_atom_forge_pop(&ps->forge, &frame);
    ui->write_function(ui->controller, 0, lv2_atom_total_size(msg),
                       ps->uris.atom_eventTransfer, msg);
}

void controller_callback(void *w_, void* user_data) {
    Widget_t *w = (Widget_t*)w_;
    const LV2_URID urid = *(const LV2_URID*)w->parent_struct;
    if (w->data == -4) {
        file_load_response(w, user_data);
    } else {
        send_controller_message(w, urid);
    }
}

static void dummy_callback(void *w_, void* user_data) {
}

void set_ctl_val_from_host(Widget_t *w, float value) {
    xevfunc store = w->func.value_changed_callback;
    w->func.value_changed_callback = dummy_callback;
    adj_set_value(w->adj, value);
    w->func.value_changed_callback = *(*store);
}
#endif

void first_loop(X11_UI *ui) {
    notify_dsp(ui);
}

void plugin_value_changed(X11_UI *ui, Widget_t *w, PortIndex index) {
    // do special stuff when needed
}

void plugin_set_window_size(int *w,int *h,const char * plugin_uri) {
    (*w) = 500; //set initial width of main window
    (*h) = 309; //set initial height of main window
}

const char* plugin_set_name() {
    return "Neural Amp Modeler"; //set plugin name to display on UI
}

void plugin_create_controller_widgets(X11_UI *ui, const char * plugin_uri) {
    set_costum_theme(ui->win);

#ifdef USE_ATOM
    X11_UI_Private_t *ps =(X11_UI_Private_t*)malloc(sizeof(X11_UI_Private_t));
    ui->private_ptr = (void*)ps;
    map_x11ui_uris(ui->map, &ps->uris);
    lv2_atom_forge_init(&ps->forge, ui->map);
    const X11LV2URIs* uris = &ps->uris;
    ps->filename = strdup("None");
    ps->dir_name = NULL;
#endif

#ifdef __linux__
    widget_set_dnd_aware(ui->win);
#endif
    ui->win->func.dnd_notify_callback = dnd_load_response;

    ui->widget[0] = add_lv2_file_button (ui->widget[0], ui->win, -4, "Neural Model", ui, 30,  254, 60, 30);
#ifdef USE_ATOM
    ui->widget[0]->parent_struct = (void*)&uris->neural_model;
    ui->widget[0]->func.user_callback = controller_callback;
#endif

    ui->widget[1] = add_lv2_knob (ui->widget[1], ui->win, 4, "Input", ui, 55,  80, 120, 140);
    set_adjustment(ui->widget[1]->adj, 0.0, 0.0, -20.0, 20.0, 0.2, CL_CONTINUOS);
    set_widget_color(ui->widget[1], 0, 0, 0.3, 0.55, 0.91, 1.0);
    set_widget_color(ui->widget[1], 0, 3,  0.682, 0.686, 0.686, 1.0);

    ui->widget[2] = add_lv2_knob (ui->widget[2], ui->win, 5, "Output ", ui, 325,  80, 120, 140);
    set_adjustment(ui->widget[2]->adj, 0.0, 0.0, -20.0, 20.0, 0.2, CL_CONTINUOS);
    set_widget_color(ui->widget[2], 0, 0, 0.3, 0.55, 0.91, 1.0);
    set_widget_color(ui->widget[2], 0, 3,  0.682, 0.686, 0.686, 1.0);

}

void plugin_cleanup(X11_UI *ui) {
#ifdef USE_ATOM
    X11_UI_Private_t *ps = (X11_UI_Private_t*)ui->private_ptr;
    free(ps->filename);
    free(ps->dir_name);
#endif
    // clean up used sources when needed
}

#ifdef USE_ATOM
Widget_t *get_widget_from_urid(X11_UI *ui, const LV2_URID urid) {
    int i = 0;
    for(; i<CONTROLS; i++) {
        if (*(const LV2_URID*)ui->widget[i]->parent_struct == urid) {
            return ui->widget[i];
        }
    }
    return NULL;
}

static inline const LV2_Atom* read_set_file(const X11LV2URIs* uris, X11_UI *ui,
                                            const LV2_Atom_Object* obj) {
    if (obj->body.otype != uris->patch_Set) {
        return NULL;
    }
    const LV2_Atom* property = NULL;
    lv2_atom_object_get(obj, uris->patch_property, &property, 0);
    if (property == NULL) return NULL;
    Widget_t *w = get_widget_from_urid(ui,((LV2_Atom_URID*)property)->body);
    if (!w || (property->type != uris->atom_URID)) {
        return NULL;
    }
    const LV2_Atom* file_path = NULL;
    lv2_atom_object_get(obj, uris->patch_value, &file_path, 0);
    if (!file_path || (file_path->type != uris->atom_Path)) {
        return NULL;
    }
    return file_path;
}
#endif

void plugin_port_event(LV2UI_Handle handle, uint32_t port_index,
                        uint32_t buffer_size, uint32_t format,
                        const void * buffer) {
#ifdef USE_ATOM
    X11_UI* ui = (X11_UI*)handle;
    X11_UI_Private_t *ps = (X11_UI_Private_t*)ui->private_ptr;
    const X11LV2URIs* uris = &ps->uris;
    if (format == ps->uris.atom_eventTransfer) {
        const LV2_Atom* atom = (LV2_Atom*)buffer;
        if (atom->type == ps->uris.atom_Object) {
            const LV2_Atom_Object* obj      = (LV2_Atom_Object*)atom;
            if (obj->body.otype == uris->patch_Set) {
                const LV2_Atom*  file_uri = read_set_file(uris, ui, obj);
                if (file_uri) {
                    const char* uri = (const char*)LV2_ATOM_BODY(file_uri);
                    if (strlen(uri)) {
                        if (strcmp(uri, (const char*)ps->filename) !=0) {
                            free(ps->filename);
                            ps->filename = NULL;
                            ps->filename = strdup(uri);
                            free(ps->dir_name);
                            ps->dir_name = NULL;
                            ps->dir_name = strdup(dirname((char*)uri));
                            FileButton *filebutton = (FileButton*)ui->widget[0]->private_struct;
                            filebutton->path = ps->dir_name;
                            expose_widget(ui->win);
                        }
                    }
                } else {
                    const LV2_Atom* value = NULL;
                    const LV2_Atom* property = NULL;
                    lv2_atom_object_get(obj, uris->patch_value, &value, 
                                    uris->patch_property, &property, 0);
                    if (value == NULL) return;
                    if (property == NULL) return;
                    Widget_t *w = get_widget_from_urid(ui,((LV2_Atom_URID*)property)->body);
                    if (w) {
                        if (value->type == uris->atom_Float ) {
                            float* val = (float*)LV2_ATOM_BODY(value);
                            set_ctl_val_from_host(w, (*val));
                        } else if (value->type == uris->atom_Int ) {
                            int* val = (int*)LV2_ATOM_BODY(value);
                            set_ctl_val_from_host(w, (float)(*val));
                        }else if (value->type == uris->atom_Bool ) {
                            int* val = (int*)LV2_ATOM_BODY(value);
                            set_ctl_val_from_host(w, (float)(*val));
                        }
                    }
                }
            }
        }
    }
#endif
    // port value change message from host
    // do special stuff when needed
}

