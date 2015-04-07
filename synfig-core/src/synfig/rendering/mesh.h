/* === S Y N F I G ========================================================= */
/*!	\file synfig/rendering/mesh.h
**	\brief Mesh Header
**
**	$Id$
**
**	\legal
**	......... ... 2015 Ivan Mahonin
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_RENDERING_MESH_H
#define __SYNFIG_RENDERING_MESH_H

/* === H E A D E R S ======================================================= */

#include "primitive.h"
#include "surface.h"
#include "renderingtask.h"
#include "../matrix.h"

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig
{
namespace rendering
{

class MeshBase: public Primitive
{
public:
	typedef etl::handle<MeshBase> Handle;

private:
	Color color;
	Surface::Handle surface;
	RenderingTask::Handle task;
	Matrix resolution_transfrom;

public:
	Color get_color() const { return color; }
	void set_color(const Color &x);

	Surface::Handle get_surface() const { return surface; }
	void set_surface(const Surface::Handle &x);

	RenderingTask::Handle get_task() const { return task; }
	void set_task(const RenderingTask::Handle &x);

	const Matrix& get_resolution_transfrom() const { return resolution_transfrom; }
	void set_resolution_transfrom(const Matrix &x);

	void apply_common_data(const MeshBase &data);
	void changed_common_data();
};

class Mesh: public MeshBase
{
public:
	typedef etl::handle<Mesh> Handle;

	enum Fields {
		FIELD_COLOR          = 1 << 0,
		FIELD_TEXCOORDS      = 1 << 1,
		FIELD_SIDE           = 1 << 2
	};

	enum {
		FIELD_COUNT          = 1 << 3
	};

	struct Vertex { Point position; };
	struct VertexC { Point position; Color color; };
	struct VertexT { Point position; Point tex_coords; };
	struct VertexTC { Point position; Point tex_coords; Color color; };
	struct VertexS { Point position; bool side; VertexS(): side() { } };
	struct VertexCS { Point position; Color color; bool side; VertexCS(): side() { } };
	struct VertexTS { Point position; Point tex_coords; bool side; VertexTS(): side() { } };
	struct VertexTCS { Point position; Point tex_coords; Color color; bool side; VertexTCS(): side() { } };

	struct Triangle
	{
		int vertices[3];
		Triangle() { memset(vertices, 0, sizeof(vertices)); }
	};

	static const size_t sizes[FIELD_COUNT];

private:
	int vertex_fields;
	std::vector<char> vertices;
	std::vector<Triangle> triangles;

public:
	Mesh();
	~Mesh();

	// vertices

	int get_vertex_fields() const { return vertex_fields; }
	void set_vertex_fields(int x);

	int get_vertices_count() const
		{ return vertices.size()/get_vertex_size(); }
	void set_vertices_count(int x);

	size_t get_vertex_size() const
		{ return sizes[get_vertex_fields()]; }
	size_t get_all_vertices_size() const
		{ return get_vertices_count()*get_vertex_size(); }

	const void* get_vertex_pointer() const
		{ return vertices.empty() ? NULL : &vertices.front(); }
	void* get_editable_vertex_pointer();

	template<typename T>
	T* get_vertices() const
	{
		assert(sizeof(T) == get_vertex_size());
		return (T*)get_vertex_pointer();
	}

	template<typename T>
	T* get_editable_vertices()
	{
		assert(sizeof(T) == get_vertex_size());
		return (T*)get_editable_vertex_pointer();
	}

	template<typename T>
	const T& get_vertex(int index) const
	{
		assert(index >= 0 && index < get_vertices_count());
		return get_vertices<T>()[index];
	}

	template<typename T>
	const T& get_editable_vertex(int index)
	{
		assert(index >= 0 && index < get_vertices_count());
		return get_editable_vertices<T>()[index];
	}

	template<typename T>
	void set_vertex(int index, const T &x)
	{
		get_editable_vertex<T>(index) = x;
	}

	// triangles

	int get_triangles_count() const
		{ return triangles.size(); }
	void set_triangles_count(int x);

	size_t get_all_triangles_size() const
		{ return get_triangles_count()*sizeof(Triangle); }
	const void* get_triangles_pointer() const
		{ return triangles.empty() ? NULL : &triangles.front(); }
	void* get_editable_triangles_pointer();

	Triangle* get_triangles() const
		{ return (Triangle*)get_triangles_pointer(); }
	Triangle* get_editable_triangles()
		{ return (Triangle*)get_editable_triangles_pointer(); }

	const Triangle& get_triangle(int index) const
	{
		assert(index >= 0 && index < get_triangles_count());
		return get_triangles()[index];
	}

	const Triangle& get_editable_triangle(int index)
	{
		assert(index >= 0 && index < get_triangles_count());
		return get_editable_triangles()[index];
	}

	void set_triangle(int index, const Triangle &x)
		{ get_editable_triangle(index) = x; }
};

} /* end namespace rendering */
} /* end namespace synfig */

/* -- E N D ----------------------------------------------------------------- */

#endif
