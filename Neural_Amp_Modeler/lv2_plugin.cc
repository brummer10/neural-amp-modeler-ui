/*
 *                           0BSD 
 * 
 *                    BSD Zero Clause License
 * 
 *  Copyright (c) 2019 Hermann Meyer
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.

 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */


#include "lv2_plugin.h"

/*---------------------------------------------------------------------
-----------------------------------------------------------------------    
                the main LV2 handle->XWindow
-----------------------------------------------------------------------
----------------------------------------------------------------------*/

void boxShadowInset(cairo_t* const cr, int x, int y, int width, int height, bool fill) {
    cairo_pattern_t *pat = cairo_pattern_create_linear (x, y, x + width, y);
    cairo_pattern_add_color_stop_rgba
        (pat, 1, 0.33, 0.33, 0.33, 1.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.98, 0.33 * 0.6, 0.33 * 0.6, 0.33 * 0.6, 0.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.05, 0.05 * 2.0, 0.05 * 2.0, 0.05 * 2.0, 0.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0, 0.05, 0.05, 0.05, 1.0);
    cairo_set_source(cr, pat);
    if (fill) cairo_fill_preserve (cr);
    else cairo_paint (cr);
    cairo_pattern_destroy (pat);
    pat = NULL;
    pat = cairo_pattern_create_linear (x, y, x, y + height);
    cairo_pattern_add_color_stop_rgba
        (pat, 1, 0.33, 0.33, 0.33, 1.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.9, 0.33 * 0.6, 0.33 * 0.6, 0.33 * 0.6, 0.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.1, 0.05 * 2.0, 0.05 * 2.0, 0.05 * 2.0, 0.0);
    cairo_pattern_add_color_stop_rgba 
        (pat, 0, 0.05, 0.05, 0.05, 1.0);
    cairo_set_source(cr, pat);
    if (fill) cairo_fill_preserve (cr);
    else cairo_paint (cr);
    cairo_pattern_destroy (pat);
}

void boxShadowOutset(cairo_t* const cr, int x, int y, int width, int height, bool fill) {
    cairo_pattern_t *pat = cairo_pattern_create_linear (x, y, x + width, y);
    cairo_pattern_add_color_stop_rgba
        (pat, 0,0.33,0.33,0.33, 1.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.03,0.33 * 0.6,0.33 * 0.6,0.33 * 0.6, 0.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.99, 0.05 * 2.0, 0.05 * 2.0, 0.05 * 2.0, 0.0);
    cairo_pattern_add_color_stop_rgba 
        (pat, 1, 0.05, 0.05, 0.05, 1.0);
    cairo_set_source(cr, pat);
    if (fill) cairo_fill_preserve (cr);
    else cairo_paint (cr);
    cairo_pattern_destroy (pat);
    pat = NULL;
    pat = cairo_pattern_create_linear (x, y, x, y + height);
    cairo_pattern_add_color_stop_rgba
        (pat, 0,0.33,0.33,0.33, 1.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.03,0.33 * 0.6,0.33 * 0.6,0.33 * 0.6, 0.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.97, 0.05 * 2.0, 0.05 * 2.0, 0.05 * 2.0, 0.0);
    cairo_pattern_add_color_stop_rgba
        (pat, 1, 0.05, 0.05, 0.05, 1.0);
    cairo_set_source(cr, pat);
    if (fill) cairo_fill_preserve (cr);
    else cairo_paint (cr);
    cairo_pattern_destroy (pat);
}

void round_rectangle(cairo_t *cr,float x, float y, float width, float height, float round) {
    float r = height* round;
    cairo_new_path (cr);
    cairo_arc(cr, x+r, y+r, r, M_PI, 3*M_PI/2);
    cairo_arc(cr, x+width-1-r, y+r, r, 3*M_PI/2, 0);
    cairo_arc(cr, x+width-1-r, y+height-1-r, r, 0, M_PI/2);
    cairo_arc(cr, x+r, y+height-1-r, r, M_PI/2, M_PI);
    cairo_close_path(cr);
}

char* utf8crop(char* dst, const char* src, size_t sizeDest ) {
    if( sizeDest ){
        size_t sizeSrc = strlen(src);
        while( sizeSrc >= sizeDest ){
            const char* lastByte = src + sizeSrc;
            while( lastByte-- > src )
                if((*lastByte & 0xC0) != 0x80)
                    break;
            sizeSrc = lastByte - src;
        }
        memcpy(dst, src, sizeSrc);
        dst[sizeSrc] = '\0';
    }
    return dst;
}

// draw the window
static void draw_window(void *w_, void* user_data) {
    Widget_t *w = (Widget_t*)w_;
    set_pattern(w,&w->color_scheme->selected,&w->color_scheme->normal,BACKGROUND_);
    cairo_paint (w->crb);
    round_rectangle(w->crb, 10 * w->app->hdpi, 10 * w->app->hdpi,
        w->width-20 * w->app->hdpi, w->height-20 * w->app->hdpi, 0.08);
    boxShadowOutset(w->crb,10 * w->app->hdpi,10 * w->app->hdpi,
        w->width-20 * w->app->hdpi,w->height-20 * w->app->hdpi, true);
    cairo_stroke (w->crb);

#ifndef HIDE_NAME
    cairo_text_extents_t extents;
    use_text_color_scheme(w, NORMAL_);
    cairo_set_font_size (w->crb, w->app->big_font+8);
    cairo_text_extents(w->crb,w->label , &extents);
    double tw = extents.width/2.0;
#endif

    widget_set_scale(w);
    cairo_move_to (w->crb, 50 * w->app->hdpi, w->scale.init_height-65 * w->app->hdpi );
    cairo_line_to (w->crb, 450 * w->app->hdpi, w->scale.init_height-65 * w->app->hdpi );
    use_text_color_scheme(w, NORMAL_);
    cairo_stroke (w->crb);
    cairo_set_source_rgba(w->crb, 0.1, 0.1, 0.1, 1);
    round_rectangle(w->crb, 100 * w->app->hdpi, w->scale.init_height-55 * w->app->hdpi,
                                            350 * w->app->hdpi, 30 * w->app->hdpi, 0.5);
    cairo_fill_preserve (w->crb);
    boxShadowInset(w->crb,100 * w->app->hdpi,w->scale.init_height-55 * w->app->hdpi,
                                            350 * w->app->hdpi, 30 * w->app->hdpi, true);
    cairo_fill (w->crb);
    if (w->image) {
        cairo_set_source_surface (w->crb, w->image, 0, 0);
        cairo_paint (w->crb);
    }
    use_text_color_scheme(w, get_color_state(w));
#ifdef USE_ATOM
    X11_UI* ui = (X11_UI*)w->parent_struct;
    X11_UI_Private_t *ps = (X11_UI_Private_t*)ui->private_ptr;
    if (strlen(ps->filename)) {
        char label[124];
        memset(label, '\0', sizeof(char)*124);
        cairo_text_extents_t extents_f;
        cairo_set_font_size (w->crb, w->app->normal_font);
        cairo_text_extents(w->crb, basename(ps->filename), &extents_f);
        
        if (extents_f.width > 350 * w->app->hdpi-10) {
            int slen = strlen(basename(ps->filename));
            int len = ((350 * w->app->hdpi-5)/(extents.width/slen));
            utf8crop(label,basename(ps->filename), min(slen-4,len-3));
            strcat(label,"...");
            tooltip_set_text(ui->widget[0],basename(ps->filename));
        } else {
            strcpy(label, basename(ps->filename));
            ui->widget[0]->flags &= ~HAS_TOOLTIP;
            hide_tooltip(ui->widget[0]);
        }

        cairo_text_extents(w->crb, label, &extents_f);
        double twf = extents_f.width/2.0;
        cairo_move_to (w->crb, max(5,(w->scale.init_width*0.5)-twf), w->scale.init_height-35 * w->app->hdpi );
        cairo_show_text(w->crb, label);       
    }
#endif
#ifndef HIDE_NAME
    cairo_set_font_size (w->crb, w->app->big_font+8);
    cairo_move_to (w->crb, (w->scale.init_width*0.5)-tw, w->scale.init_y+50 * w->app->hdpi);
    cairo_show_text(w->crb, w->label);
    cairo_move_to (w->crb, 50 * w->app->hdpi, w->scale.init_y+55 * w->app->hdpi);
    cairo_line_to (w->crb, 450 * w->app->hdpi, w->scale.init_y+55 * w->app->hdpi);
    use_text_color_scheme(w, NORMAL_);
    cairo_stroke (w->crb);
#endif
    widget_reset_scale(w);
    cairo_new_path (w->crb);
}

// if controller value changed send notify to host
static void value_changed(void *w_, void* user_data) {
    Widget_t *w = (Widget_t*)w_;
    X11_UI* ui = (X11_UI*)w->parent_struct;
    float v = adj_get_value(w->adj);
    ui->write_function(ui->controller,w->data,sizeof(float),0,&v);
    plugin_value_changed(ui, w, (PortIndex)w->data);
}


void knobShadowOutset(cairo_t* const cr, int width, int height) {
    int x = 0;
    int y = 0;
    cairo_pattern_t *pat = cairo_pattern_create_linear (x, y, x + width, y + height);
    cairo_pattern_add_color_stop_rgba
        (pat, 0, 0.33, 0.33, 0.33, 1);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.45, 0.33 * 0.6, 0.33 * 0.6, 0.33 * 0.6, 0.4);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.65, 0.05 * 2.0, 0.05 * 2.0, 0.05 * 2.0, 0.4);
    cairo_pattern_add_color_stop_rgba 
        (pat, 1, 0.05, 0.05, 0.05, 1);
    cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
    cairo_set_source(cr, pat);
    cairo_fill_preserve (cr);
    cairo_pattern_destroy (pat);
}

void knobShadowInset(cairo_t* const cr, int width, int height) {
    int x = 0;
    int y = 0;
    cairo_pattern_t* pat = cairo_pattern_create_linear (x, y, x + width, y + height);
    cairo_pattern_add_color_stop_rgba
        (pat, 1, 0.33, 0.33, 0.33, 1);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.65, 0.33 * 0.6, 0.33 * 0.6, 0.33 * 0.6, 0.4);
    cairo_pattern_add_color_stop_rgba
        (pat, 0.55, 0.05 * 2.0, 0.05 * 2.0, 0.05 * 2.0, 0.4);
    cairo_pattern_add_color_stop_rgba
        (pat, 0, 0.05, 0.05, 0.05, 1);
    cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
    cairo_set_source(cr, pat);
    cairo_fill (cr);
    cairo_pattern_destroy (pat);
}

static void draw_my_knob(void *w_, void* user_data) {
    Widget_t *w = (Widget_t*)w_;    

    /** get size for the knob **/
    const int width = w->width;
    const int height = w->height - (w->height * 0.15);

    const int grow = (width > height) ? height:width;
    const int knob_x = grow-1;
    const int knob_y = grow-1;

    const int knobx = (width - knob_x) * 0.5;
    const int knobx1 = width* 0.5;

    const int knoby = (height - knob_y) * 0.5;
    const int knoby1 = height * 0.5;

    /** get geometric values for the knob **/
    const double scale_zero = 20 * (M_PI/180); // defines "dead zone"
    const double state = adj_get_state(w->adj);
    const double angle = scale_zero + state * 2 * (M_PI - scale_zero);

    const double pointer_off =knob_x/3.5;
    const double radius = min(knob_x-pointer_off, knob_y-pointer_off) / 2;
    const double lengh_x = (knobx+radius+pointer_off/2) - radius * sin(angle);
    const double lengh_y = (knoby+radius+pointer_off/2) + radius * cos(angle);
    const double radius_x = (knobx+radius+pointer_off/2) - radius * sin(angle);
    const double radius_y = (knoby+radius+pointer_off/2) + radius * cos(angle);

    /** draw the knob **/
    cairo_push_group (w->crb);

    cairo_arc(w->crb,knobx1, knoby1, knob_x/2.1, 0, 2 * M_PI );
    knobShadowOutset(w->crb, width, height);
    cairo_stroke_preserve (w->crb);
    cairo_new_path (w->crb);

    cairo_arc(w->crb,knobx1, knoby1, knob_x/2.4, 0, 2 * M_PI );
    knobShadowOutset(w->crb, width, height);
    cairo_set_line_width(w->crb,knobx1/10);
    cairo_set_source_rgba(w->crb, 0.05, 0.05, 0.05, 1);
    cairo_stroke_preserve (w->crb);
    cairo_new_path (w->crb);

    cairo_arc(w->crb,knobx1, knoby1, knob_x/3.1, 0, 2 * M_PI );
    use_bg_color_scheme(w, get_color_state(w));
    cairo_fill_preserve (w->crb);
    knobShadowInset(w->crb, width, height);
    cairo_new_path (w->crb);

    /** create a rotating pointer on the kob**/
    cairo_set_line_cap(w->crb, CAIRO_LINE_CAP_ROUND); 
    cairo_set_line_join(w->crb, CAIRO_LINE_JOIN_BEVEL);
    cairo_move_to(w->crb, radius_x, radius_y);
    cairo_line_to(w->crb,lengh_x,lengh_y);
    cairo_set_line_width(w->crb,knobx1/10);
    use_fg_color_scheme(w, NORMAL_);
    cairo_stroke_preserve(w->crb);
    cairo_new_path (w->crb);

    /** create a indicator ring around the knob **/
    const double add_angle = 90 * (M_PI / 180.);
    cairo_new_sub_path(w->crb);
    use_fg_color_scheme(w, NORMAL_);
    cairo_set_line_width(w->crb,knobx1/25);
    cairo_arc (w->crb, knobx1, knoby1, knob_x/2.4,
            add_angle + scale_zero, add_angle + angle);
    cairo_stroke(w->crb);

    /** show value on the kob**/
    use_text_color_scheme(w, get_color_state(w));
    cairo_text_extents_t extents;
    cairo_select_font_face (w->crb, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (w->crb, height * 0.1);
    char s[17];
    char sa[17];
    float value = adj_get_value(w->adj);
    const char* format[] = {"%.1f", "%.2f", "%.3f"};
    if (fabs(w->adj->step)>0.99) {
        snprintf(s, 16,"%d",  (int) value);
    } else if (fabs(w->adj->step)>0.09) {
        snprintf(s, 16, format[1-1], value);
    } else {
        snprintf(s, 16, format[2-1], value);
    }
    snprintf(sa, strlen(s),"%s",  "000000000000000");
    cairo_text_extents(w->crb, sa, &extents);
    int wx = extents.width * 0.5;
    cairo_text_extents(w->crb, s, &extents);
    cairo_move_to (w->crb, knobx1 - wx, knoby1+extents.height/2);
    cairo_show_text(w->crb, s);
    cairo_new_path (w->crb);

    /** show label below the knob**/
    use_text_color_scheme(w, get_color_state(w));
    cairo_set_font_size (w->crb, w->app->normal_font+4);
    cairo_text_extents(w->crb,w->label , &extents);
    cairo_move_to (w->crb, (width*0.5)-(extents.width/2), height + (height * 0.15)-(extents.height*0.1));
    cairo_show_text(w->crb, w->label);
    cairo_new_path (w->crb);

    cairo_pop_group_to_source (w->crb);
    cairo_paint (w->crb);
}


Widget_t* add_lv2_knob(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height) {
    w = add_knob(p, label, x, y, width, height);
    w->parent_struct = ui;
    w->data = index;
    w->func.expose_callback = draw_my_knob;
    w->func.value_changed_callback = value_changed;
    return w;
}

static void my_fdialog_response(void *w_, void* user_data) {
    Widget_t *w = (Widget_t*)w_;
    FileButton *filebutton = (FileButton *)w->private_struct;
    if(user_data !=NULL) {
        char *tmp = strdup(*(const char**)user_data);
        free(filebutton->last_path);
        filebutton->last_path = NULL;
        filebutton->last_path = strdup(dirname(tmp));
        filebutton->path = filebutton->last_path;
        free(tmp);
    }
    w->func.user_callback(w,user_data);
    filebutton->is_active = false;
    adj_set_value(w->adj,0.0);
}

static void my_fbutton_callback(void *w_, void* user_data) {
    Widget_t *w = (Widget_t*)w_;
    FileButton *filebutton = (FileButton *)w->private_struct;
    if (w->flags & HAS_POINTER && adj_get_value(w->adj)){
        filebutton->w = open_file_dialog(w,filebutton->path,filebutton->filter);
#ifdef __linux__
        Atom wmStateAbove = XInternAtom(w->app->dpy, "_NET_WM_STATE_ABOVE", 1 );
        Atom wmNetWmState = XInternAtom(w->app->dpy, "_NET_WM_STATE", 1 );
        XChangeProperty(w->app->dpy, filebutton->w->widget, wmNetWmState, XA_ATOM, 32, 
            PropModeReplace, (unsigned char *) &wmStateAbove, 1); 
#elif defined _WIN32
        os_set_transient_for_hint(w, filebutton->w);
#endif
        filebutton->is_active = true;
    } else if (w->flags & HAS_POINTER && !adj_get_value(w->adj)){
        if(filebutton->is_active)
            destroy_widget(filebutton->w,w->app);
    }
}

static void my_fbutton_mem_free(void *w_, void* user_data) {
    Widget_t *w = (Widget_t*)w_;
    FileButton *filebutton = (FileButton *)w->private_struct;
    free(filebutton->last_path);
    filebutton->last_path = NULL;
    free(filebutton);
    filebutton = NULL;
}

Widget_t *add_my_file_button(Widget_t *parent, int x, int y, int width, int height,
                           const char *path, const char *filter) {
    FileButton *filebutton = (FileButton*)malloc(sizeof(FileButton));
    filebutton->path = path;
    filebutton->filter = filter;
    filebutton->last_path = NULL;
    filebutton->w = NULL;
    filebutton->is_active = false;
    Widget_t *fbutton = add_toggle_button(parent, ". . .", x, y, width, height);
    fbutton->private_struct = filebutton;
    fbutton->flags |= HAS_MEM;
    fbutton->scale.gravity = CENTER;
    fbutton->func.mem_free_callback = my_fbutton_mem_free;
    fbutton->func.value_changed_callback = my_fbutton_callback;
    fbutton->func.dialog_callback = my_fdialog_response;
    return fbutton;
}


Widget_t* add_lv2_file_button(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height) {
    w = add_my_file_button(p, x, y, width, height, "", ".nam");
    w->data = index;
    return w;
}

// init the xwindow and return the LV2UI handle
static LV2UI_Handle instantiate(const LV2UI_Descriptor * descriptor,
            const char * plugin_uri, const char * bundle_path,
            LV2UI_Write_Function write_function,
            LV2UI_Controller controller, LV2UI_Widget * widget,
            const LV2_Feature * const * features) {

    X11_UI* ui = (X11_UI*)malloc(sizeof(X11_UI));

    if (!ui) {
        fprintf(stderr,"ERROR: failed to instantiate plugin with URI %s\n", plugin_uri);
        return NULL;
    }

    ui->parentXwindow = 0;
    ui->private_ptr = NULL;
    ui->need_resize = 1;
    ui->loop_counter = 2;

    int i = 0;
    for(;i<CONTROLS;i++)
        ui->widget[i] = NULL;
    i = 0;
    for(;i<GUI_ELEMENTS;i++)
        ui->elem[i] = NULL;

    i = 0;
    for (; features[i]; ++i) {
        if (!strcmp(features[i]->URI, LV2_UI__parent)) {
            ui->parentXwindow = features[i]->data;
        } else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
            ui->resize = (LV2UI_Resize*)features[i]->data;
        } else if (!strcmp(features[i]->URI, LV2_URID_URI "#map")) {
            ui->map = (LV2_URID_Map*)features[i]->data;
        }
    }

    if (ui->parentXwindow == NULL)  {
        fprintf(stderr, "ERROR: Failed to open parentXwindow for %s\n", plugin_uri);
        free(ui);
        return NULL;
    }

    // init Xputty
    main_init(&ui->main);
    int w = 1;
    int h = 1;
    plugin_set_window_size(&w,&h,plugin_uri);
    // create the toplevel Window on the parentXwindow provided by the host
    ui->win = create_window(&ui->main, (Window)ui->parentXwindow, 0, 0, w, h);
    ui->win->parent_struct = ui;
    ui->win->label = plugin_set_name();
    // connect the expose func
    ui->win->func.expose_callback = draw_window;
    // create controller widgets
    plugin_create_controller_widgets(ui,plugin_uri);
    // map all widgets into the toplevel Widget_t
    widget_show_all(ui->win);
    // set the widget pointer to the X11 Window from the toplevel Widget_t
    *widget = (void*)ui->win->widget;
    // request to resize the parentXwindow to the size of the toplevel Widget_t
    if (ui->resize){
        ui->resize->ui_resize(ui->resize->handle, ui->win->width, ui->win->height);
    }
    // store pointer to the host controller
    ui->controller = controller;
    // store pointer to the host write function
    ui->write_function = write_function;
    
    return (LV2UI_Handle)ui;
}

// cleanup after usage
static void cleanup(LV2UI_Handle handle) {
    X11_UI* ui = (X11_UI*)handle;
    plugin_cleanup(ui);
    // Xputty free all memory used
    main_quit(&ui->main);
    free(ui->private_ptr);
    free(ui);
}

static void null_callback(void *w_, void* user_data) {
    
}

/*---------------------------------------------------------------------
-----------------------------------------------------------------------    
                        LV2 interface
-----------------------------------------------------------------------
----------------------------------------------------------------------*/

// port value change message from host
static void port_event(LV2UI_Handle handle, uint32_t port_index,
                        uint32_t buffer_size, uint32_t format,
                        const void * buffer) {
    X11_UI* ui = (X11_UI*)handle;
    float value = *(float*)buffer;
    int i=0;
    for (;i<CONTROLS;i++) {
        if (ui->widget[i] && port_index == (uint32_t)ui->widget[i]->data) {
            // prevent event loop between host and plugin
            xevfunc store = ui->widget[i]->func.value_changed_callback;
            ui->widget[i]->func.value_changed_callback = null_callback;
            // Xputty check if the new value differs from the old one
            // and set new one, when needed
            adj_set_value(ui->widget[i]->adj, value);
            // activate value_change_callback back
            ui->widget[i]->func.value_changed_callback = store;
        }
   }
   plugin_port_event(handle, port_index, buffer_size, format, buffer);
}

// LV2 idle interface to host
static int ui_idle(LV2UI_Handle handle) {
    X11_UI* ui = (X11_UI*)handle;
    if (ui->need_resize == 1) {
        ui->need_resize = 2;
    } else if (ui->need_resize == 2) {
        int i=0;
        for (;i<CONTROLS;i++) {
            os_move_window(ui->main.dpy, ui->widget[i], ui->widget[i]->x, ui->widget[i]->y);
        }
        ui->need_resize = 0;
    }
    // Xputty event loop setup to run one cycle when called
    run_embedded(&ui->main);
#ifdef USE_ATOM
    if (ui->loop_counter > 0) {
        ui->loop_counter--;
        if (ui->loop_counter == 0)
            first_loop(ui);
    }
#endif
    return 0;
}

// LV2 resize interface to host
static int ui_resize(LV2UI_Feature_Handle handle, int w, int h) {
    X11_UI* ui = (X11_UI*)handle;
    // Xputty sends configure event to the toplevel widget to resize itself
    if (ui) send_configure_event(ui->win,0, 0, w, h);
    return 0;
}

// connect idle and resize functions to host
static const void* extension_data(const char* uri) {
    static const LV2UI_Idle_Interface idle = { ui_idle };
    static const LV2UI_Resize resize = { 0 ,ui_resize };
    if (!strcmp(uri, LV2_UI__idleInterface)) {
        return &idle;
    }
    if (!strcmp(uri, LV2_UI__resize)) {
        return &resize;
    }
    return NULL;
}

static const LV2UI_Descriptor descriptors[] = {
    {PLUGIN_UI_URI,instantiate,cleanup,port_event,extension_data},
#ifdef PLUGIN_UI_URI2
    {PLUGIN_UI_URI2,instantiate,cleanup,port_event,extension_data},
#endif
};

LV2_SYMBOL_EXPORT
const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index) {
    if (index >= sizeof(descriptors) / sizeof(descriptors[0])) {
        return NULL;
    }
    return descriptors + index;
}

