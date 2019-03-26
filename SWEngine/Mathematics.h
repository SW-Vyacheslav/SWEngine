#pragma once

namespace SWEngine
{
	namespace Mathematics
	{
		class Vec2D
		{
		public:
			Vec2D();
			Vec2D(const float& x, const float& y);
			Vec2D(const Vec2D& vec);

			void SetX(const float& x);
			void SetY(const float& y);
			void SetW(const float& w);
			float GetX() const;
			float GetY() const;
			float GetW() const;

		private:
			float x;
			float y;
			float w;
		};

		class Mat3x3
		{
		public:
			Mat3x3();
			Mat3x3(const Mat3x3& mat);

			void SetElementAt(const int& row, const int& column, const float& val);
			float GetElementAt(const int& row, const int& column) const;

		private:
			float grid[3][3];
		};

		class Convert
		{
		public:
			static float DegreesToRadians(const float& deg);
		};

		void vecsum(Vec2D& out, const Vec2D& vec1, const Vec2D& vec2);
		void vecsum(Vec2D& vec1, const Vec2D& vec2);
		void vecsub(Vec2D& out, const Vec2D& vec1, const Vec2D& vec2);
		void vecsub(Vec2D& vec1, const Vec2D& vec2);
		void vecmul(Vec2D& out, const Vec2D& vec, const float& num);
		void vecmul(Vec2D& out, const Vec2D& vec, const Mat3x3& mat);
		void vecmul(Vec2D& vec, const Mat3x3& mat);
		void matmul(Mat3x3& out, const Mat3x3& mat1, const Mat3x3& mat2);
		void matmul(Mat3x3& mat1, const Mat3x3& mat2);
		float veclen(const Vec2D& vec);
		void vecnormalize(Vec2D& out);
		void setrotationmat(Mat3x3& out, const float& angle);
		void settranslatemat(Mat3x3& out, const float& x, const float& y);
		void setscalemat(Mat3x3& out, const float& x, const float& y);
		Vec2D vectoscreen(const Vec2D& vec, const float& cx, const float& cy);
		Vec2D screentovec(const float& x, const float& y, const float& cx, const float& cy);
		void vecrotate(Vec2D& out, const float& angle);
		void vecscale(Vec2D& out, const float& x, const float& y);
		void vectranslate(Vec2D& out, const float& x, const float& y);
	}
}