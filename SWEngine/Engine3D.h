#pragma once
#include "Engine.h"

struct range
{
	float from = 0;
	float to = 0;
};
struct vec2d
{
	float x = 0;
	float y = 0;
};
struct vec3d
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};
struct triangle
{
	vec3d vertices[3];
	COLORREF color;
};
struct mat4x4
{
	float grid[4][4] = { 0 };
};
struct mesh
{
	std::vector<triangle> triangles;

	bool LoadFromObjectFile(std::string sFileName)
	{
		std::ifstream f(sFileName);
		if (!f.is_open()) return false;

		std::vector<vec3d> vectors;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				vectors.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				triangles.push_back({ vectors[f[0] - 1],vectors[f[1] - 1],vectors[f[2] - 1] });
			}
		}

		return true;
	}
};

float degreestoradians(const float& degrees)
{
	return degrees * (3.14f / 180.0f);
}
float map(const float& value, const range& old_range, const range& new_range)
{
	return new_range.from + ((new_range.to - new_range.from) * ((value - old_range.from) / (old_range.to - old_range.from)));
}

void vectopoint(vec3d& out)
{
	out.x /= out.w;
	out.y /= out.w;
	out.z /= out.w;
}

void vecsum(vec3d& out, const vec3d& vec1, const vec3d& vec2)
{
	out.x = vec1.x + vec2.x;
	out.y = vec1.y + vec2.y;
	out.z = vec1.z + vec2.z;
}
void vecsum(vec3d& vec1, const vec3d& vec2)
{
	vec1.x = vec1.x + vec2.x;
	vec1.y = vec1.y + vec2.y;
	vec1.z = vec1.z + vec2.z;
}
void vecsub(vec3d& out, const vec3d& vec1, const vec3d& vec2)
{
	out.x = vec1.x - vec2.x;
	out.y = vec1.y - vec2.y;
	out.z = vec1.z - vec2.z;
}
void vecmul(vec3d& out, const vec3d& vec, const float& num)
{
	out.x = vec.x * num;
	out.y = vec.y * num;
	out.z = vec.z * num;
}
void vecmul(vec3d& out, const vec3d& vec, const mat4x4& mat)
{
	out.x = (vec.x * mat.grid[0][0]) + (vec.y * mat.grid[1][0]) + (vec.z * mat.grid[2][0]) + (vec.w * mat.grid[3][0]);
	out.y = (vec.x * mat.grid[0][1]) + (vec.y * mat.grid[1][1]) + (vec.z * mat.grid[2][1]) + (vec.w * mat.grid[3][1]);
	out.z = (vec.x * mat.grid[0][2]) + (vec.y * mat.grid[1][2]) + (vec.z * mat.grid[2][2]) + (vec.w * mat.grid[3][2]);
	out.w = (vec.x * mat.grid[0][3]) + (vec.y * mat.grid[1][3]) + (vec.z * mat.grid[2][3]) + (vec.w * mat.grid[3][3]);
}
void matmul(mat4x4& out, const mat4x4& mat1, const mat4x4& mat2)
{
	float sum = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				sum += mat1.grid[i][k] * mat2.grid[k][j];
			}
			out.grid[i][j] = sum;
			sum = 0;
		}
	}
}
void matidentity(mat4x4& out)
{
	out.grid[0][0] = 1.0f;
	out.grid[1][1] = 1.0f;
	out.grid[2][2] = 1.0f;
	out.grid[3][3] = 1.0f;
}
void crossp(vec3d& out, const vec3d& vec1, const vec3d& vec2)
{
	out.x = (vec1.y * vec2.z) - (vec1.z * vec2.y);
	out.y = (vec1.z * vec2.x) - (vec1.x * vec2.z);
	out.z = (vec1.x * vec2.y) - (vec1.y * vec2.x);
}
float dotp(const vec3d& vec1, const vec3d& vec2)
{
	return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}
float veclen(const vec3d& vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}
void vecnormalize(vec3d& out, const vec3d& vec)
{
	out.x = vec.x / veclen(vec);
	out.y = vec.y / veclen(vec);
	out.z = vec.z / veclen(vec);
}

void setprojmat(mat4x4& out, const float& ratio, const float& fov, const float& znear, const float& zfar)
{
	float temp = 1 / tanf(degreestoradians(fov * 0.5f));

	out.grid[0][0] = temp * ratio;
	out.grid[1][1] = temp;
	out.grid[2][2] = zfar / (zfar - znear);
	out.grid[2][3] = 1.0f;
	out.grid[3][2] = (-zfar * znear) / (zfar - znear);
}
void settranslatemat(mat4x4& out, const float& x = 0, const float& y = 0, const float& z = 0)
{
	out.grid[0][0] = 1.0f;
	out.grid[1][1] = 1.0f;
	out.grid[2][2] = 1.0f;
	out.grid[3][3] = 1.0f;
	out.grid[3][0] = x;
	out.grid[3][1] = y;
	out.grid[3][2] = z;
}
void setscalemat(mat4x4& out, const float& x = 1, const float& y = 1, const float& z = 1)
{
	out.grid[0][0] = x;
	out.grid[1][1] = y;
	out.grid[2][2] = z;
	out.grid[3][3] = 1.0f;
}
void setrotationxmat(mat4x4& out, const float& angle)
{
	out.grid[0][0] = 1.0f;
	out.grid[1][1] = cosf(angle);
	out.grid[1][2] = sinf(angle);
	out.grid[2][1] = -sinf(angle);
	out.grid[2][2] = cosf(angle);
	out.grid[3][3] = 1.0f;
}
void setrotationymat(mat4x4& out, const float& angle)
{
	out.grid[0][0] = cosf(angle);
	out.grid[0][2] = -sinf(angle);
	out.grid[1][1] = 1.0f;
	out.grid[2][0] = sinf(angle);
	out.grid[2][2] = cosf(angle);
	out.grid[3][3] = 1.0f;
}
void setrotationzmat(mat4x4& out, const float& angle)
{
	out.grid[0][0] = cosf(angle);
	out.grid[0][1] = sinf(angle);
	out.grid[1][0] = -sinf(angle);
	out.grid[1][1] = cosf(angle);
	out.grid[2][2] = 1.0f;
	out.grid[3][3] = 1.0f;
}

COLORREF GetColor(float lum)
{
	COLORREF col;

	int pixel_bw = (int)(13.0f*lum);

	switch (pixel_bw)
	{
	case 0: col = RGB(0, 0, 0); break;

	case 1: col = RGB(169, 169, 169); break;
	case 2: col = RGB(169, 169, 169); break;
	case 3: col = RGB(169, 169, 169); break;
	case 4: col = RGB(169, 169, 169); break;

	case 5: col = RGB(128, 128, 128); break;
	case 6: col = RGB(128, 128, 128); break;
	case 7: col = RGB(128, 128, 128); break;
	case 8: col = RGB(128, 128, 128); break;

	case 9:  col = RGB(255, 255, 255); break;
	case 10: col = RGB(255, 255, 255); break;
	case 11: col = RGB(255, 255, 255); break;
	case 12: col = RGB(255, 255, 255); break;

	default: col = RGB(0, 0, 0); break;
	}

	return col;
}

class Engine3D : public Engine
{
public:
	VOID OnCreate() override
	{
		object_mesh.LoadFromObjectFile("mesh.obj");
		float ratio = (float)GetClientHeight() / (float)GetClientWidth();
		setprojmat(proj_mat, ratio, 90.0f, 0.1f, 100.0f);

		rot_angle = 0;

		ShowFps(true);
	}

	VOID OnUpdate(float fdeltaTime) override
	{
		//Key events
		if (GetKeyInfo(KeyCode::F)->KeyDown)
		{
			if (IsFpsShowed()) ShowFps(false);
			else ShowFps(true);
		}

		//Math
		std::vector<triangle> triangles_to_draw;

		rot_angle += 1.0f * fdeltaTime;

		setrotationzmat(rotz_mat,rot_angle);
		setrotationxmat(rotx_mat,rot_angle*0.5f);
		settranslatemat(trans_mat,0.0f,0.0f,5.0f);

		matidentity(world_mat);

		mat4x4 temp_world_mat;
		matmul(temp_world_mat,rotz_mat,rotx_mat);
		matmul(world_mat,temp_world_mat,trans_mat);

		for (triangle tr : object_mesh.triangles)
		{
			triangle tr_projected, tr_transformed;

			vecmul(tr_transformed.vertices[0],tr.vertices[0],world_mat);
			vecmul(tr_transformed.vertices[1],tr.vertices[1],world_mat);
			vecmul(tr_transformed.vertices[2],tr.vertices[2],world_mat);

			vec3d normal, line1, line2, normalized_normal;

			vecsub(line1,tr_transformed.vertices[1],tr_transformed.vertices[0]);
			vecsub(line2,tr_transformed.vertices[2],tr_transformed.vertices[0]);
			crossp(normal,line1,line2);
			vecnormalize(normalized_normal,normal);

			vecsub(camera_ray_vec,tr_transformed.vertices[0],camera_vec);

			if( dotp(normalized_normal, camera_ray_vec) < 0.0f )
			{
				vec3d light_dir = { 0.0f, 0.0f, -1.0f };
				vec3d normalized_light_dir;
				vecnormalize(normalized_light_dir,light_dir);

				tr_transformed.color = GetColor(max(0.1f,dotp(normalized_normal,normalized_light_dir)));

				vecmul(tr_projected.vertices[0],tr_transformed.vertices[0],proj_mat);
				vecmul(tr_projected.vertices[1],tr_transformed.vertices[1],proj_mat);
				vecmul(tr_projected.vertices[2],tr_transformed.vertices[2],proj_mat);

				tr_projected.color = tr_transformed.color;

				vectopoint(tr_projected.vertices[0]);
				vectopoint(tr_projected.vertices[1]);
				vectopoint(tr_projected.vertices[2]);

				vec3d view_offset_vec = {1.0f,1.0f,0.0f};

				vecsum(tr_projected.vertices[0],view_offset_vec);
				vecsum(tr_projected.vertices[1],view_offset_vec);
				vecsum(tr_projected.vertices[2],view_offset_vec);

				tr_projected.vertices[0].x *= 0.5f*(float)GetClientWidth();
				tr_projected.vertices[0].y *= 0.5f*(float)GetClientHeight();
				tr_projected.vertices[1].x *= 0.5f*(float)GetClientWidth();
				tr_projected.vertices[1].y *= 0.5f*(float)GetClientHeight();
				tr_projected.vertices[2].x *= 0.5f*(float)GetClientWidth();
				tr_projected.vertices[2].y *= 0.5f*(float)GetClientHeight();

				triangles_to_draw.push_back(tr_projected);
			}
		}

		std::sort
		(
			triangles_to_draw.begin(),
			triangles_to_draw.end(),
			[](triangle& t1, triangle& t2)
			{
				return ((t1.vertices[0].z + t1.vertices[1].z + t1.vertices[2].z) / 3.0f) > ((t2.vertices[0].z + t2.vertices[1].z + t2.vertices[2].z) / 3.0f);
			}
		);

		//Drawing
		FillWindow(Color::BLACK);

		for (triangle tr : triangles_to_draw)
		{
			FillTriangle
			(
				tr.vertices[0].x,
				tr.vertices[0].y,
				tr.vertices[1].x,
				tr.vertices[1].y,
				tr.vertices[2].x,
				tr.vertices[2].y,
				tr.color
			);
			DrawTriangle
			(
				tr.vertices[0].x,
				tr.vertices[0].y,
				tr.vertices[1].x,
				tr.vertices[1].y,
				tr.vertices[2].x,
				tr.vertices[2].y,
				Color::BLACK
			);
		}
	}

private:
	mesh object_mesh;

	mat4x4 proj_mat;
	mat4x4 rotx_mat;
	mat4x4 roty_mat;
	mat4x4 rotz_mat;
	mat4x4 trans_mat;
	mat4x4 scale_mat;

	mat4x4 world_mat;

	vec3d camera_vec;
	vec3d camera_ray_vec;

	float rot_angle;
};