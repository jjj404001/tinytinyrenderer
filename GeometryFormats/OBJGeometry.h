// filename : OBJGeometry.h
// author   : Jaejun Jang
// email    : jjj404001@gmail.com
// brief    : Geometry file structure for wavefront OBJ format.
#pragma once
#include <vector>
#include <string>
#include "../Math/Vector.h"
class OBJ_Geometry
{
	// Reference : http://paulbourke.net/dataformats/obj/
	// File structure
	/*
	Vertex data

		o       geometric vertices(v)
		o       texture vertices(vt)
		o       vertex normals(vn)
		o       parameter space vertices(vp)
					Free - form curve / surface attributes
		o       rational or non - rational forms of curve or surface type :
					basis matrix, Bezier, B - spline, Cardinal, Taylor(cstype)
		o       degree(deg)
		o       basis matrix(bmat)
		o       step size(step)


		Elements
		o       point(p)
		o       line(l)
		o       face(f)
		o       curve(curv)
		o       2D curve(curv2)
		o       surface(surf)


		Free - form curve / surface body statements
		o       parameter values(parm)
		o       outer trimming loop(trim)
		o       inner trimming loop(hole)
		o       special curve(scrv)
		o       special point(sp)
		o       end statement(end)

		Connectivity between free - form surfaces
		o       connect(con)

		Grouping
		o       group name(g)
		o       smoothing group(s)
		o       merging group(mg)
		o       object name(o)

		Display / render attributes
		o       bevel interpolation(bevel)
		o       color interpolation(c_interp)
		o       dissolve interpolation(d_interp)
		o       level of detail(lod)
		o       material name(usemtl)
		o       material library(mtllib)
		o       shadow casting(shadow_obj)
		o       ray tracing(trace_obj)
		o       curve approximation technique(ctech)
		o       surface approximation technique(stech)
		*/

	std::string material_library_name_;
	// std::string object_name[]; Not sure we need it or not.
	std::vector<Vec3f> geometric_vertices;
	std::vector<Vec3f> texture_vertices;
	std::vector<Vec3f> vertex_normals;

public: 
	OBJ_Geometry(std::string _filename);
	bool LoadFromOBJFile(std::string _filename);
};