/* -*- c-file-style: "ruby" -*- */
/*
 *  Copyright (C) 2008-2010  Kouhei Sutou <kou@clear-code.com>
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../toolkit/rb-milter-core-private.h"
#include "rb-milter-manager-private.h"

#define SELF(self) (MILTER_MANAGER_CHILDREN(RVAL2GOBJ(self)))

static VALUE
initialize (VALUE self, VALUE configuration)
{
    G_INITIALIZE(self, milter_manager_children_new(RVAL2GOBJ(configuration)));
    return Qnil;
}

static VALUE
length (VALUE self)
{
    return UINT2NUM(milter_manager_children_length(SELF(self)));
}

static VALUE
add_child (VALUE self, VALUE child)
{
    milter_manager_children_add_child(SELF(self), RVAL2GOBJ(child));
    return Qnil;
}

static VALUE
add_child_less_than (VALUE self, VALUE child)
{
    add_child(self, child);
    return self;
}

static void
cb_each (gpointer data, gpointer user_data)
{
    MilterManagerChild *child = data;
    rb_yield(GOBJ2RVAL(child));
}

static VALUE
each (VALUE self)
{
    milter_manager_children_foreach(SELF(self), cb_each, NULL);
    return Qnil;
}

static VALUE
get_children (VALUE self)
{
    return GLIST2ARY(milter_manager_children_get_children(SELF(self)));
}

static VALUE
get_quitted_children (VALUE self)
{
    return GLIST2ARY(milter_manager_children_get_quitted_children(SELF(self)));
}

static VALUE
get_smtp_client_address (VALUE self)
{
    struct sockaddr *address;
    socklen_t address_length;

    if (milter_manager_children_get_smtp_client_address(SELF(self),
							&address,
							&address_length)) {
	return ADDRESS2RVAL(address, address_length);
    } else {
	return Qnil;
    }
}

static void
mark (gpointer data)
{
    MilterManagerChildren *children = data;

    g_list_foreach((GList *)milter_manager_children_get_children(children),
		   (GFunc)rbgobj_gc_mark_instance, NULL);
}

void
Init_milter_manager_children (void)
{
    VALUE rb_cMilterManagerChildren;

    rb_cMilterManagerChildren =
	G_DEF_CLASS_WITH_GC_FUNC(MILTER_TYPE_MANAGER_CHILDREN,
				 "Children",
				 rb_mMilterManager,
				 mark,
				 NULL);

    rb_define_method(rb_cMilterManagerChildren, "initialize",
                     initialize, 1);
    rb_define_method(rb_cMilterManagerChildren, "length", length, 0);
    rb_define_method(rb_cMilterManagerChildren, "add", add_child, 1);
    rb_define_method(rb_cMilterManagerChildren, "<<", add_child_less_than, 1);
    rb_define_method(rb_cMilterManagerChildren, "each", each, 0);
    rb_define_method(rb_cMilterManagerChildren, "children", get_children, 0);
    rb_define_method(rb_cMilterManagerChildren, "quitted_children",
		     get_quitted_children, 0);
    rb_define_method(rb_cMilterManagerChildren, "smtp_client_address",
		     get_smtp_client_address, 0);
}
