#pragma once
#include "Engine.h"
#include <algorithm>

struct Vector3D
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

struct Triangle
{
	Vector3D vertices[3];
	COLORREF color;
};

struct Mesh
{
	std::vector<Triangle> triangles;

	bool LoadFromObjectFile(std::string sFileName)
	{
		std::ifstream f(sFileName);
		if (!f.is_open()) return false;

		std::vector<Vector3D> vectors;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				Vector3D v;
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

struct Matrix4x4
{
	float grid[4][4] = { 0 };
};

class Test : public Engine
{
	Vector3D Matrix_MultiplyVector(Matrix4x4& mat, Vector3D& v)
	{
		Vector3D vec;
		vec.x = v.x * mat.grid[0][0] + v.y * mat.grid[1][0] + v.z * mat.grid[2][0] + v.w * mat.grid[3][0];
		vec.y = v.x * mat.grid[0][1] + v.y * mat.grid[1][1] + v.z * mat.grid[2][1] + v.w * mat.grid[3][1];
		vec.z = v.x * mat.grid[0][2] + v.y * mat.grid[1][2] + v.z * mat.grid[2][2] + v.w * mat.grid[3][2];
		vec.w = v.x * mat.grid[0][3] + v.y * mat.grid[1][3] + v.z * mat.grid[2][3] + v.w * mat.grid[3][3];

		return vec;
	}

	Matrix4x4 Matrix_MakeIdentity()
	{
		Matrix4x4 matrix;
		matrix.grid[0][0] = 1.0f;
		matrix.grid[1][1] = 1.0f;
		matrix.grid[2][2] = 1.0f;
		matrix.grid[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 Matrix_MakeRotationX(float fAngleRad)
	{
		Matrix4x4 matrix;
		matrix.grid[0][0] = 1.0f;
		matrix.grid[1][1] = cosf(fAngleRad);
		matrix.grid[1][2] = sinf(fAngleRad);
		matrix.grid[2][1] = -sinf(fAngleRad);
		matrix.grid[2][2] = cosf(fAngleRad);
		matrix.grid[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 Matrix_MakeRotationY(float fAngleRad)
	{
		Matrix4x4 matrix;
		matrix.grid[0][0] = cosf(fAngleRad);
		matrix.grid[0][2] = sinf(fAngleRad);
		matrix.grid[2][0] = -sinf(fAngleRad);
		matrix.grid[1][1] = 1.0f;
		matrix.grid[2][2] = cosf(fAngleRad);
		matrix.grid[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 Matrix_MakeRotationZ(float fAngleRad)
	{
		Matrix4x4 matrix;
		matrix.grid[0][0] = cosf(fAngleRad);
		matrix.grid[0][1] = sinf(fAngleRad);
		matrix.grid[1][0] = -sinf(fAngleRad);
		matrix.grid[1][1] = cosf(fAngleRad);
		matrix.grid[2][2] = 1.0f;
		matrix.grid[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 Matrix_MakeTranslation(float x, float y, float z)
	{
		Matrix4x4 matrix;
		matrix.grid[0][0] = 1.0f;
		matrix.grid[1][1] = 1.0f;
		matrix.grid[2][2] = 1.0f;
		matrix.grid[3][3] = 1.0f;
		matrix.grid[3][0] = x;
		matrix.grid[3][1] = y;
		matrix.grid[3][2] = z;
		return matrix;
	}

	Matrix4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
	{
		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
		Matrix4x4 matrix;
		matrix.grid[0][0] = fAspectRatio * fFovRad;
		matrix.grid[1][1] = fFovRad;
		matrix.grid[2][2] = fFar / (fFar - fNear);
		matrix.grid[3][2] = (-fFar * fNear) / (fFar - fNear);
		matrix.grid[2][3] = 1.0f;
		matrix.grid[3][3] = 0.0f;
		return matrix;
	}

	Matrix4x4 Matrix_MultiplyMatrix(Matrix4x4& m1, Matrix4x4& m2)
	{
		Matrix4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.grid[r][c] = m1.grid[r][0] * m2.grid[0][c] + m1.grid[r][1] * m2.grid[1][c] + m1.grid[r][2] * m2.grid[2][c] + m1.grid[r][3] * m2.grid[3][c];
		return matrix;
	}

	Matrix4x4 Matrix_PointAt(Vector3D& pos, Vector3D& target, Vector3D& up)
	{
		// Calculate new forward direction
		Vector3D newForward = Vector_Sub(target, pos);
		newForward = Vector_Normalise(newForward);

		// Calculate new Up direction
		Vector3D a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
		Vector3D newUp = Vector_Sub(up, a);
		newUp = Vector_Normalise(newUp);

		// New Right direction is easy, its just cross product
		Vector3D newRight = Vector_CrossProduct(newUp, newForward);

		// Construct Dimensioning and Translation Matrix
		Matrix4x4 matrix;
		matrix.grid[0][0] = newRight.x;
		matrix.grid[0][1] = newRight.y;
		matrix.grid[0][2] = newRight.z;
		matrix.grid[0][3] = 0.0f;
		matrix.grid[1][0] = newUp.x;
		matrix.grid[1][1] = newUp.y;
		matrix.grid[1][2] = newUp.z;
		matrix.grid[1][3] = 0.0f;
		matrix.grid[2][0] = newForward.x;
		matrix.grid[2][1] = newForward.y;
		matrix.grid[2][2] = newForward.z;
		matrix.grid[2][3] = 0.0f;
		matrix.grid[3][0] = pos.x;
		matrix.grid[3][1] = pos.y;
		matrix.grid[3][2] = pos.z;
		matrix.grid[3][3] = 1.0f;

		return matrix;

	}

	Matrix4x4 Matrix_QuickInverse(Matrix4x4& m) // Only for Rotation/Translation Matrices
	{
		Matrix4x4 matrix;
		matrix.grid[0][0] = m.grid[0][0];
		matrix.grid[0][1] = m.grid[1][0];
		matrix.grid[0][2] = m.grid[2][0];
		matrix.grid[0][3] = 0.0f;
		matrix.grid[1][0] = m.grid[0][1];
		matrix.grid[1][1] = m.grid[1][1];
		matrix.grid[1][2] = m.grid[2][1];
		matrix.grid[1][3] = 0.0f;
		matrix.grid[2][0] = m.grid[0][2];
		matrix.grid[2][1] = m.grid[1][2];
		matrix.grid[2][2] = m.grid[2][2];
		matrix.grid[2][3] = 0.0f;
		matrix.grid[3][0] = -(m.grid[3][0] * matrix.grid[0][0] + m.grid[3][1] * matrix.grid[1][0] + m.grid[3][2] * matrix.grid[2][0]);
		matrix.grid[3][1] = -(m.grid[3][0] * matrix.grid[0][1] + m.grid[3][1] * matrix.grid[1][1] + m.grid[3][2] * matrix.grid[2][1]);
		matrix.grid[3][2] = -(m.grid[3][0] * matrix.grid[0][2] + m.grid[3][1] * matrix.grid[1][2] + m.grid[3][2] * matrix.grid[2][2]);
		matrix.grid[3][3] = 1.0f;

		return matrix;
	}

	Vector3D Vector_Add(Vector3D& v1, Vector3D& v2)
	{
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	Vector3D Vector_Sub(Vector3D& v1, Vector3D& v2)
	{
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	Vector3D Vector_Mul(Vector3D& v1, float k)
	{
		return { v1.x * k, v1.y * k, v1.z * k };
	}

	Vector3D Vector_Div(Vector3D& v1, float k)
	{
		return { v1.x / k, v1.y / k, v1.z / k };
	}

	float Vector_DotProduct(Vector3D& v1, Vector3D& v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	float Vector_Magnitude(Vector3D& v)
	{
		return sqrtf(Vector_DotProduct(v, v));
	}

	Vector3D Vector_Normalise(Vector3D& v)
	{
		float l = Vector_Magnitude(v);
		return { v.x / l, v.y / l, v.z / l };
	}

	Vector3D Vector_CrossProduct(Vector3D& v1, Vector3D& v2)
	{
		Vector3D v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	}

	Vector3D Vector_IntersectPlane(Vector3D &plane_p, Vector3D &plane_n, Vector3D &lineStart, Vector3D &lineEnd)
	{
		plane_n = Vector_Normalise(plane_n);
		float plane_d = -Vector_DotProduct(plane_n, plane_p);
		float ad = Vector_DotProduct(lineStart, plane_n);
		float bd = Vector_DotProduct(lineEnd, plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		Vector3D lineStartToEnd = Vector_Sub(lineEnd, lineStart);
		Vector3D lineToIntersect = Vector_Mul(lineStartToEnd, t);
		return Vector_Add(lineStart, lineToIntersect);
	}

	int Triangle_ClipAgainstPlane(Vector3D plane_p, Vector3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2)
	{
		// Make sure plane normal is indeed normal
		plane_n = Vector_Normalise(plane_n);

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto dist = [&](Vector3D &p)
		{
			Vector3D n = Vector_Normalise(p);
			return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
		};

		// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		Vector3D* inside_points[3];
		int nInsidePointCount = 0;
		Vector3D* outside_points[3];
		int nOutsidePointCount = 0;

		// Get signed distance of each point in triangle to plane
		float d0 = dist(in_tri.vertices[0]);
		float d1 = dist(in_tri.vertices[1]);
		float d2 = dist(in_tri.vertices[2]);

		if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.vertices[0]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.vertices[0]; }
		if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.vertices[1]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.vertices[1]; }
		if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.vertices[2]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.vertices[2]; }

		// Now classify triangle points, and break the input triangle into
		// smaller output triangles if required. There are four possible
		// outcomes...

		if (nInsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole triangle
			// It ceases to exist

			return 0; // No returned triangles are valid
		}

		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through
			out_tri1 = in_tri;

			return 1; // Just the one returned original triangle is valid
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			out_tri1.color = in_tri.color;

			// The inside point is valid, so keep that...
			out_tri1.vertices[0] = *inside_points[0];

			// but the two new points are at the locations where the
			// original sides of the triangle (lines) intersect with the plane
			out_tri1.vertices[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
			out_tri1.vertices[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

			return 1; // Return the newly formed single triangle
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			out_tri1.color = in_tri.color;

			out_tri2.color = in_tri.color;

			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri1.vertices[0] = *inside_points[0];
			out_tri1.vertices[1] = *inside_points[1];
			out_tri1.vertices[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the
			// triangle and the plane, and the newly created point above
			out_tri2.vertices[0] = *inside_points[1];
			out_tri2.vertices[1] = out_tri1.vertices[2];
			out_tri2.vertices[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

			return 2; // Return two newly formed triangles which form a quad
		}
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

	VOID OnCreate() override
	{
		f_mesh.LoadFromObjectFile("monkey.obj");

		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)GetClientHeight() / (float)GetClientWidth();

		f_matProj = Matrix_MakeProjection(fFov, fAspectRatio, fNear, fFar);
	}

	VOID OnUpdate(float fdeltaTime) override
	{
		if(GetKeyInfo(KeyCode::F)->KeyDown)
		{
			if(IsFpsShowed()) ShowFps(false);
			else ShowFps(true);
		}

		SetCurrentBrushColor(RGB(0,0,0));
		FillWindow();

		f_fTheta += 1.0f * fdeltaTime;

		f_matRotZ = Matrix_MakeRotationZ(f_fTheta*0.5f + 3.14f);
		f_matRotX = Matrix_MakeRotationX(f_fTheta);

		f_matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

		f_matWorld = Matrix_MakeIdentity();
		f_matWorld = Matrix_MultiplyMatrix(f_matRotZ, f_matRotX);
		f_matWorld = Matrix_MultiplyMatrix(f_matWorld, f_matTrans);

		Vector3D vUp = { 0.0f, 1.0f, 0.0f };
		Vector3D vTarget = { 0.0f, 0.0f, 1.0f };
		Matrix4x4 matCameraRot = Matrix_MakeRotationY(f_fYaw);
		f_vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
		vTarget = Vector_Add(f_vCamera, f_vLookDir);

		Matrix4x4 matCamera = Matrix_PointAt(f_vCamera, vTarget, vUp);
		Matrix4x4 matView = Matrix_QuickInverse(matCamera);

		for (Triangle triangle : f_mesh.triangles)
		{
			Triangle triProjected, triTransformed, triViewed;

			triTransformed.vertices[0] = Matrix_MultiplyVector(f_matWorld, triangle.vertices[0]);
			triTransformed.vertices[1] = Matrix_MultiplyVector(f_matWorld, triangle.vertices[1]);
			triTransformed.vertices[2] = Matrix_MultiplyVector(f_matWorld, triangle.vertices[2]);

			Vector3D normal, line1, line2;

			line1 = Vector_Sub(triTransformed.vertices[1], triTransformed.vertices[0]);
			line2 = Vector_Sub(triTransformed.vertices[2], triTransformed.vertices[0]);
			normal = Vector_CrossProduct(line1, line2);
			normal = Vector_Normalise(normal);

			f_vCameraRay = Vector_Sub(triTransformed.vertices[0], f_vCamera);

			if (Vector_DotProduct(normal, f_vCameraRay) < 0.0f)
			{
				f_vLightDir = { 0.0f, 1.0f, -1.0f };
				f_vLightDir = Vector_Normalise(f_vLightDir);

				triTransformed.color = GetColor(max(0.1f, Vector_DotProduct(f_vLightDir, normal)));

				triViewed.vertices[0] = Matrix_MultiplyVector(matView, triTransformed.vertices[0]);
				triViewed.vertices[1] = Matrix_MultiplyVector(matView, triTransformed.vertices[1]);
				triViewed.vertices[2] = Matrix_MultiplyVector(matView, triTransformed.vertices[2]);

				triProjected.vertices[0] = Matrix_MultiplyVector(f_matProj, triViewed.vertices[0]);
				triProjected.vertices[1] = Matrix_MultiplyVector(f_matProj, triViewed.vertices[1]);
				triProjected.vertices[2] = Matrix_MultiplyVector(f_matProj, triViewed.vertices[2]);
				triProjected.color = triTransformed.color;

				triProjected.vertices[0] = Vector_Div(triProjected.vertices[0], triProjected.vertices[0].w);
				triProjected.vertices[1] = Vector_Div(triProjected.vertices[1], triProjected.vertices[1].w);
				triProjected.vertices[2] = Vector_Div(triProjected.vertices[2], triProjected.vertices[2].w);

				Vector3D vOffsetView = { 1.0f, 1.0f, 0.0f };
				triProjected.vertices[0] = Vector_Add(triProjected.vertices[0], vOffsetView);
				triProjected.vertices[1] = Vector_Add(triProjected.vertices[1], vOffsetView);
				triProjected.vertices[2] = Vector_Add(triProjected.vertices[2], vOffsetView);

				triProjected.vertices[0].x *= 0.5f*(float)GetClientWidth();
				triProjected.vertices[0].y *= 0.5f*(float)GetClientHeight();
				triProjected.vertices[1].x *= 0.5f*(float)GetClientWidth();
				triProjected.vertices[1].y *= 0.5f*(float)GetClientHeight();
				triProjected.vertices[2].x *= 0.5f*(float)GetClientWidth();
				triProjected.vertices[2].y *= 0.5f*(float)GetClientHeight();

				f_vecTrianglesToRaster.push_back(triProjected);
			}
		}

		std::sort
		(
			f_vecTrianglesToRaster.begin(),
			f_vecTrianglesToRaster.end(),
			[](Triangle& t1, Triangle& t2)
			{
				return ((t1.vertices[0].z + t1.vertices[1].z + t1.vertices[2].z) / 3.0f) > ((t2.vertices[0].z + t2.vertices[1].z + t2.vertices[2].z) / 3.0f);
			}
		);

		for (Triangle& triProjected : f_vecTrianglesToRaster)
		{
			SetCurrentBrushColor(triProjected.color);
			DrawTriangle
			(
				triProjected.vertices[0].x,
				triProjected.vertices[0].y,
				triProjected.vertices[1].x,
				triProjected.vertices[1].y,
				triProjected.vertices[2].x,
				triProjected.vertices[2].y
			);
		}

		f_vecTrianglesToRaster.clear();
	}

	VOID OnDestroy() override
	{

	}

public:
	Test()
	{
		f_fTheta = 0.0f;
	}

private:
	Mesh f_mesh;
	Matrix4x4 f_matProj;
	Matrix4x4 f_matRotZ;
	Matrix4x4 f_matRotX;
	Matrix4x4 f_matWorld;
	Matrix4x4 f_matTrans;
	Vector3D f_vCamera;
	Vector3D f_vCameraRay;
	Vector3D f_vLookDir;
	Vector3D f_vLightDir;
	std::vector<Triangle> f_vecTrianglesToRaster;
	float f_fTheta;
	float f_fYaw;
};