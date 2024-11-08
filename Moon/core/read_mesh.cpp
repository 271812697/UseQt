#include<stdlib.h>
#include <bit>
#include "core/read_mesh.h"
#include "core/exceptions.h"
namespace MOON {

	void read_obj(SurfaceMesh& mesh, const std::filesystem::path& file)
	{
		std::array<char, 200> s;
		float x, y, z;
		std::vector<Vertex> vertices;
		std::vector<TexCoord> all_tex_coords;
		std::vector<int>
			halfedge_tex_idx;
		HalfedgeProperty<TexCoord> tex_coords =
			mesh.halfedge_property<TexCoord>("h:tex");
		bool with_tex_coord = false;


		FILE* in = fopen(file.string().c_str(), "r");
		if (!in)
			throw IOException("Failed to open file: " + file.string());


		memset(s.data(), 0, 200);


		while (in && !feof(in) && fgets(s.data(), 200, in))
		{

			if (s[0] == '#' || isspace(s[0]))
				continue;

			else if (strncmp(s.data(), "v ", 2) == 0)
			{
				if (sscanf(s.data(), "v %f %f %f", &x, &y, &z))
				{
					mesh.add_vertex(Point(x, y, z));
				}
			}

			else if (strncmp(s.data(), "vn ", 3) == 0)
			{
				if (sscanf(s.data(), "vn %f %f %f", &x, &y, &z))
				{
					// problematic as it can be either a vertex property when interpolated
					// or a halfedge property for hard edges
				}
			}

			else if (strncmp(s.data(), "vt ", 3) == 0)
			{
				if (sscanf(s.data(), "vt %f %f", &x, &y))
				{
					all_tex_coords.emplace_back(x, y);
				}
			}

			else if (strncmp(s.data(), "f ", 2) == 0)
			{
				int component(0);
				bool end_of_vertex(false);
				char* p0, * p1(s.data() + 1);

				vertices.clear();
				halfedge_tex_idx.clear();

				while (*p1 == ' ')
					++p1;

				while (p1)
				{
					p0 = p1;

					while (*p1 != '/' && *p1 != '\r' && *p1 != '\n' && *p1 != ' ' &&
						*p1 != '\0')
						++p1;

					if (*p1 != '/')
					{
						end_of_vertex = true;
					}

					if (*p1 != '\0')
					{
						*p1 = '\0';
						p1++;
					}

					if (*p1 == '\0' || *p1 == '\n')
					{
						p1 = nullptr;
					}

					if (*p0 != '\0')
					{
						switch (component)
						{
						case 0:
						{
							int idx = atoi(p0);
							if (idx < 0)
								idx = mesh.n_vertices() + idx + 1;
							vertices.emplace_back(idx - 1);
							break;
						}
						case 1:
						{
							int idx = atoi(p0) - 1;
							halfedge_tex_idx.push_back(idx);
							with_tex_coord = true;
							break;
						}
						case 2:
							break;
						}
					}
					++component;
					if (end_of_vertex)
					{
						component = 0;
						end_of_vertex = false;
					}
				}

				Face f;
				try
				{
					f = mesh.add_face(vertices);
				}
				catch (const TopologyException& e)
				{
					std::cerr << e.what();
				}

				if (with_tex_coord && f.is_valid())
				{
					auto h_fit = mesh.halfedges(f);
					auto h_end = h_fit;
					unsigned v_idx = 0;
					do
					{
						tex_coords[*h_fit] =
							all_tex_coords.at(halfedge_tex_idx.at(v_idx));
						++v_idx;
						++h_fit;
					} while (h_fit != h_end);
				}
			}
			memset(s.data(), 0, 200);
		}

		if (!with_tex_coord)
		{
			mesh.remove_halfedge_property(tex_coords);
		}
		fclose(in);
	}
	void read_off(SurfaceMesh& mesh, const std::filesystem::path& file)
	{
		std::array<char, 200> line;
		bool has_texcoords = false;
		bool has_normals = false;
		bool has_colors = false;
		bool has_hcoords = false;
		bool has_dim = false;
		bool is_binary = false;

		// open file (in ASCII mode)
		FILE* in = fopen(file.string().c_str(), "r");
		if (!in)
			throw IOException("Failed to open file: " + file.string());

		// read header: [ST][C][N][4][n]OFF BINARY
		char* c = fgets(line.data(), 200, in);
		assert(c != nullptr);
		c = line.data();
		if (c[0] == 'S' && c[1] == 'T')
		{
			has_texcoords = true;
			c += 2;
		}
		if (c[0] == 'C')
		{
			has_colors = true;
			++c;
		}
		if (c[0] == 'N')
		{
			has_normals = true;
			++c;
		}
		if (c[0] == '4')
		{
			has_hcoords = true;
			++c;
		}
		if (c[0] == 'n')
		{
			has_dim = true;
			++c;
		}
		if (strncmp(c, "OFF", 3) != 0)
		{
			fclose(in);
			throw IOException("Failed to parse OFF header");
		}
		c += 3;
		if (c[0] == ' ')
			++c;
		if (strncmp(c, "BINARY", 6) == 0)
		{
			is_binary = true;
			c += 6;
		}
		if (c[0] == ' ')
			++c;

		if (has_hcoords)
		{
			fclose(in);
			throw IOException("Error: Homogeneous coordinates not supported.");
		}
		if (has_dim)
		{
			fclose(in);
			throw IOException("Error: vertex dimension != 3 not supported");
		}

		// if binary: reopen file in binary mode
		if (is_binary)
		{
			fclose(in);
			in = fopen(file.string().c_str(), "rb");
			c = fgets(line.data(), 200, in);
			assert(c != nullptr);
		}

		// read as ASCII or binary
		if (is_binary)
			read_off_binary(mesh, in, has_normals, has_texcoords, has_colors, file);
		else
			read_off_ascii(mesh, in, has_normals, has_texcoords, has_colors, c);

		fclose(in);
	}
	void read_off_ascii(SurfaceMesh& mesh, FILE* in, const bool has_normals, const bool has_texcoords, const bool has_colors, char* first_line)
	{
		std::array<char, 1000> line;
		char* lp = first_line;
		int nc;
		long int i, j, idx;
		long int nv, nf, ne;
		float x, y, z, r, g, b;
		Vertex v;

		// properties
		VertexProperty<Normal> normals;
		VertexProperty<TexCoord> texcoords;
		VertexProperty<Color> colors;
		if (has_normals)
			normals = mesh.vertex_property<Normal>("v:normal");
		if (has_texcoords)
			texcoords = mesh.vertex_property<TexCoord>("v:tex");
		if (has_colors)
			colors = mesh.vertex_property<Color>("v:color");

		// read line, but skip comment lines
		while (lp && (lp[0] == '#' || lp[0] == '\n'))
		{
			lp = fgets(line.data(), 1000, in);
		}

		// #Vertices, #Faces, #Edges
		auto items = sscanf(lp, "%ld %ld %ld\n", &nv, &nf, &ne);

		if (items < 3 || nv < 1 || nf < 1 || ne < 0)
			throw IOException("Failed to parse OFF header");

		mesh.reserve(nv, std::max(3 * nv, ne), nf);

		// read vertices: pos [normal] [color] [texcoord]
		for (i = 0; i < nv && !feof(in); ++i)
		{
			// read line, but skip comment lines
			do
			{
				lp = fgets(line.data(), 1000, in);
			} while (lp && (lp[0] == '#' || lp[0] == '\n'));
			lp = line.data();

			// position
			items = sscanf(lp, "%f %f %f%n", &x, &y, &z, &nc);
			assert(items == 3);
			v = mesh.add_vertex(Point(x, y, z));
			lp += nc;

			// normal
			if (has_normals)
			{
				if (sscanf(lp, "%f %f %f%n", &x, &y, &z, &nc) == 3)
				{
					normals[v] = Normal(x, y, z);
				}
				lp += nc;
			}

			// color
			if (has_colors)
			{
				if (sscanf(lp, "%f %f %f%n", &r, &g, &b, &nc) == 3)
				{
					if (r > 1.0f || g > 1.0f || b > 1.0f)
					{
						r /= 255.0f;
						g /= 255.0f;
						b /= 255.0f;
					}
					colors[v] = Color(r, g, b);
				}
				lp += nc;
			}

			// tex coord
			if (has_texcoords)
			{
				items = sscanf(lp, "%f %f%n", &x, &y, &nc);
				assert(items == 2);
				texcoords[v][0] = x;
				texcoords[v][1] = y;
				lp += nc;
			}
		}

		// read faces: #N v[1] v[2] ... v[n-1]
		std::vector<Vertex> vertices;
		for (i = 0; i < nf; ++i)
		{
			// read line, but skip comment lines
			do
			{
				lp = fgets(line.data(), 1000, in);
			} while (lp && (lp[0] == '#' || lp[0] == '\n'));
			lp = line.data();

			// #vertices
			items = sscanf(lp, "%ld%n", &nv, &nc);
			assert(items == 1);
			if (nv < 1)
				throw IOException("Invalid index count");
			vertices.resize(nv);
			lp += nc;

			// indices
			for (j = 0; j < nv; ++j)
			{
				items = sscanf(lp, "%ld%n", &idx, &nc);
				assert(items == 1);
				if (idx < 0)
					throw IOException("Invalid index");
				vertices[j] = Vertex(idx);
				lp += nc;
			}
			try
			{
				mesh.add_face(vertices);
			}
			catch (const TopologyException& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}

	template <typename T>
	void read_binary(FILE* in, T& t, bool swap = false)
	{
		auto n_items = fread((char*)&t, 1, sizeof(t), in);

	}

	void read_off_binary(SurfaceMesh& mesh, FILE* in, const bool has_normals,
		const bool has_texcoords, const bool has_colors,
		const std::filesystem::path& file)
	{
		uint32_t i, j, idx(0);
		uint32_t nv(0), nf(0), ne(0);
		vec3 p, n;
		vec2 t;
		Vertex v;

		// binary cannot (yet) read colors
		if (has_colors)
			throw IOException("Colors not supported for binary OFF file.");

		// properties
		VertexProperty<Normal> normals;
		VertexProperty<TexCoord> texcoords;
		if (has_normals)
			normals = mesh.vertex_property<Normal>("v:normal");
		if (has_texcoords)
			texcoords = mesh.vertex_property<TexCoord>("v:tex");

		// #Vertices, #Faces, #Edges
		read_binary(in, nv);

		// Check for little endian encoding used by previous versions.
		// Swap the ordering if the total file size is smaller than the size
		// required to store all vertex coordinates.
		auto file_size = std::filesystem::file_size(file);
		bool swap = file_size < nv * 3 * 4 ? true : false;
		if (swap)
			nv = _byteswap_ulong(nv);

		read_binary(in, nf, swap);
		read_binary(in, ne, swap);
		mesh.reserve(nv, std::max(3 * nv, ne), nf);

		// read vertices: pos [normal] [color] [texcoord]
		for (i = 0; i < nv && !feof(in); ++i)
		{
			// position
			read_binary(in, p[0], swap);
			read_binary(in, p[1], swap);
			read_binary(in, p[2], swap);
			v = mesh.add_vertex((Point)p);

			// normal
			if (has_normals)
			{
				read_binary(in, n[0], swap);
				read_binary(in, n[1], swap);
				read_binary(in, n[2], swap);
				normals[v] = (Normal)n;
			}

			// tex coord
			if (has_texcoords)
			{
				read_binary(in, t[0], swap);
				read_binary(in, t[1], swap);
				texcoords[v][0] = t[0];
				texcoords[v][1] = t[1];
			}
		}

		// read faces: #N v[1] v[2] ... v[n-1]
		std::vector<Vertex> vertices;
		for (i = 0; i < nf; ++i)
		{
			read_binary(in, nv, swap);
			vertices.resize(nv);
			for (j = 0; j < nv; ++j)
			{
				read_binary(in, idx, swap);
				vertices[j] = Vertex(idx);
			}
			try
			{
				mesh.add_face(vertices);
			}
			catch (const TopologyException& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}
}