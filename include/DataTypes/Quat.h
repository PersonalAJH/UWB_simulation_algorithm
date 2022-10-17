/********************************************	
*		Original source code: SOFA			*
*		Modified by Hoeryong Jung			*
*		Date: 2009. 10. 21					*
*		contact: junghl80@kaist.ac.kr		*
/*******************************************/

#ifndef QUAT_H
#define QUAT_H

#include "Vec.h"
#include "Mat.h"
#include <math.h>
#include <assert.h>
#include <iostream>

#define RENORMCOUNT 50

class Quat
{
private:
	double q[4];

public:
	Quat()
	{
		q[0] = q[1] = q[2] = 0.0;
		q[3] = 1.0;
	};
	
	Quat(double x, double y, double z, double w)
	{
		q[0] = x;
		q[1] = y;
		q[2] = z;
		q[3] = w;
	};

	Quat( const Vec<3,double>& axis, double angle)
	{
		//axisToQuat(axis,angle);
	};

    ~Quat()
	{
	};

    static Quat identity() 
	{
        return Quat(0,0,0,1);
    };

	const double* ptr() const
    {
        return this->q;
    };

    /// Cast into a standard C array of elements.
    double* ptr()
    {
        return this->q;
    };

	/// Normalize a quaternion
    void normalize()
	{
		int i;
		double mag;
		mag = (q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
		if( mag != 0)
		{
			for (i = 0; i < 4; i++)
			{
	  			q[i] /= sqrt(mag);
			}
		}
	};

	void clear()
    {
        q[0]=0.0;
        q[1]=0.0;
        q[2]=0.0;
        q[3]=1.0;
    };

	void fromMatrix(const Mat3x3d &m)
	{
		double tr, s;

		tr = (double)(m.x().x() + m.y().y() + m.z().z());

		// check the diagonal
		if (tr > 0)
		{
			s = (float)sqrt (tr + 1);
			q[3] = s * 0.5f; // w OK
			s = 0.5f / s;
			q[0] = (double)((m.z().y() - m.y().z()) * s); // x OK
			q[1] = (double)((m.x().z() - m.z().x()) * s); // y OK
			q[2] = (double)((m.y().x() - m.x().y()) * s); // z OK
		}
		else
		{
			if (m.y().y() > m.x().x() && m.z().z() <= m.y().y())
			{
				s = (double)sqrt ((m.y().y() - (m.z().z() + m.x().x())) + 1.0f);
				q[1] = s * 0.5f; // y OK

				if (s != 0.0f)
					s = 0.5f / s;

				q[2] = (double)((m.y().z() + m.z().y()) * s); // z OK
				q[0] = (double)((m.x().y() + m.y().x()) * s); // x OK
				q[3] = (double)((m.x().z() - m.z().x()) * s); // w OK
			}
			else if ((m.y().y() <= m.x().x()  &&  m.z().z() > m.x().x())  ||  (m.z().z() > m.y().y()))
			{
				s = (double)sqrt ((m.z().z() - (m.x().x() + m.y().y())) + 1.0f);

				q[2] = s * 0.5f; // z OK

				if (s != 0.0f)
					s = 0.5f / s;

				q[0] = (double)((m.z().x() + m.x().z()) * s); // x OK
				q[1] = (double)((m.y().z() + m.z().y()) * s); // y OK
				q[3] = (double)((m.y().x() - m.x().y()) * s); // w OK
			}
			else
			{
				s = (double)sqrt ((m.x().x() - (m.y().y() + m.z().z())) + 1.0f);

				q[0] = s * 0.5f; // x OK

				if (s != 0.0f)
					s = 0.5f / s;

				q[1] = (double)((m.x().y() + m.y().x()) * s); // y OK
				q[2] = (double)((m.z().x() + m.x().z()) * s); // z OK
				q[3] = (double)((m.z().y() - m.y().z()) * s); // w OK
			}
		}
	};

	void toMatrix(Mat3x3d &m) const
    {
        m[0][0] = (double) (1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]));
        m[0][1] = (double) (2.0f * (q[0] * q[1] - q[2] * q[3]));
        m[0][2] = (double) (2.0f * (q[2] * q[0] + q[1] * q[3]));

        m[1][0] = (double) (2.0f * (q[0] * q[1] + q[2] * q[3]));
        m[1][1] = (double) (1.0f - 2.0f * (q[2] * q[2] + q[0] * q[0]));
        m[1][2] = (double) (2.0f * (q[1] * q[2] - q[0] * q[3]));

        m[2][0] = (double) (2.0f * (q[2] * q[0] - q[1] * q[3]));
        m[2][1] = (double) (2.0f * (q[1] * q[2] + q[0] * q[3]));
        m[2][2] = (double) (1.0f - 2.0f * (q[1] * q[1] + q[0] * q[0]));
    };

	 /// Apply the rotation to a given vector
    Vec3d rotate( const Vec3d& v ) const
    {
        return Vec3d(
            (double)((1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]))*v[0] + (2.0f * (q[0] * q[1] - q[2] * q[3])) * v[1] + (2.0f * (q[2] * q[0] + q[1] * q[3])) * v[2]),
            (double)((2.0f * (q[0] * q[1] + q[2] * q[3]))*v[0] + (1.0f - 2.0f * (q[2] * q[2] + q[0] * q[0]))*v[1] + (2.0f * (q[1] * q[2] - q[0] * q[3]))*v[2]),
            (double)((2.0f * (q[2] * q[0] - q[1] * q[3]))*v[0] + (2.0f * (q[1] * q[2] + q[0] * q[3]))*v[1] + (1.0f - 2.0f * (q[1] * q[1] + q[0] * q[0]))*v[2])
               );
    };

	/// Apply the inverse rotation to a given vector
    Vec3d inverseRotate( const Vec3d& v ) const
    {
        return Vec3d(
            (double)((1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]))*v[0] + (2.0f * (q[0] * q[1] + q[2] * q[3])) * v[1] + (2.0f * (q[2] * q[0] - q[1] * q[3])) * v[2]),
            (double)((2.0f * (q[0] * q[1] - q[2] * q[3]))*v[0] + (1.0f - 2.0f * (q[2] * q[2] + q[0] * q[0]))*v[1] + (2.0f * (q[1] * q[2] + q[0] * q[3]))*v[2]),
            (double)((2.0f * (q[2] * q[0] + q[1] * q[3]))*v[0] + (2.0f * (q[1] * q[2] - q[0] * q[3]))*v[1] + (1.0f - 2.0f * (q[1] * q[1] + q[0] * q[0]))*v[2])
               );
    };

	/// Given two quaternions, add them together to get a third quaternion.
    /// Adding quaternions to get a compound rotation is analagous to adding
    /// translations to get a compound translation.
	Quat operator+(const Quat &q1) const
	{
		static int	count	= 0;

		double		t1[4], t2[4], t3[4];
		double		tf[4];
		Quat		ret;

		t1[0] = q[0] * q1.q[3];
		t1[1] = q[1] * q1.q[3];
		t1[2] = q[2] * q1.q[3];

		t2[0] = q1.q[0] * q[3];
		t2[1] = q1.q[1] * q[3];
		t2[2] = q1.q[2] * q[3];

		// cross product t3 = q2 x q1
		t3[0] = (q1.q[1] * q[2]) - (q1.q[2] * q[1]);
		t3[1] = (q1.q[2] * q[0]) - (q1.q[0] * q[2]);
		t3[2] = (q1.q[0] * q[1]) - (q1.q[1] * q[0]);
		
		// end cross product
		tf[0] = t1[0] + t2[0] + t3[0];
		tf[1] = t1[1] + t2[1] + t3[1];
		tf[2] = t1[2] + t2[2] + t3[2];
		tf[3] = q[3] * q1.q[3] -
				(q[0] * q1.q[0] + q[1] * q1.q[1] + q[2] * q1.q[2]);

		ret.q[0] = tf[0];
		ret.q[1] = tf[1];
		ret.q[2] = tf[2];
		ret.q[3] = tf[3];

		if (++count > RENORMCOUNT)
		{
			count = 0;
			ret.normalize();
		}

		return ret;
	};

	Quat operator*(const Quat& q1) const
	{
		Quat	ret;

		ret.q[3] = q[3] * q1.q[3] -
					(q[0] * q1.q[0] +
					 q[1] * q1.q[1] +
					 q[2] * q1.q[2]);
		ret.q[0] = q[3] * q1.q[0] +
					q[0] * q1.q[3] +
					q[1] * q1.q[2] -
					q[2] * q1.q[1];
		ret.q[1] = q[3] * q1.q[1] +
					q[1] * q1.q[3] +
					q[2] * q1.q[0] -
					q[0] * q1.q[2];
		ret.q[2] = q[3] * q1.q[2] +
					q[2] * q1.q[3] +
					q[0] * q1.q[1] -
					q[1] * q1.q[0];

		return ret;
	};

	double& operator[](int index)
    {
        assert(index >= 0 && index < 4);
        return q[index];
    };

    const double& operator[](int index) const
    {
        assert(index >= 0 && index < 4);
        return q[index];
    };

	Quat operator*(const double& r) const
	{
	  Quat  ret;
	  ret[0] = q[0] * r;
	  ret[1] = q[1] * r;
	  ret[2] = q[2] * r;
	  ret[3] = q[3] * r;
	  return ret;
	};

	Quat operator/(const double& r) const
	{
		Quat  ret;
		ret[0] = q[0] / r;
		ret[1] = q[1] / r;
		ret[2] = q[2] / r;
		ret[3] = q[3] / r;
		return ret;
	};

	void operator*=(const double& r)
	{
		q[0] *= r;
		q[1] *= r;
		q[2] *= r;
		q[3] *= r;
	};

	void operator/=(const double& r)
	{
		q[0] /= r;
		q[1] /= r;
		q[2] /= r;
		q[3] /= r;
	};
	
	void operator+=(const Quat& q2)
	{
		static int	count	= 0;

		double t1[4], t2[4], t3[4];
		Quat q1 = (*this);
		t1[0] = q1.q[0] * q2.q[3];
		t1[1] = q1.q[1] * q2.q[3];
		t1[2] = q1.q[2] * q2.q[3];

		t2[0] = q2.q[0] * q1.q[3];
		t2[1] = q2.q[1] * q1.q[3];
		t2[2] = q2.q[2] * q1.q[3];

		// cross product t3 = q2 x q1
		t3[0] = (q2.q[1] * q1.q[2]) - (q2.q[2] * q1.q[1]);
		t3[1] = (q2.q[2] * q1.q[0]) - (q2.q[0] * q1.q[2]);
		t3[2] = (q2.q[0] * q1.q[1]) - (q2.q[1] * q1.q[0]);
		
		// end cross product
		q[0] = t1[0] + t2[0] + t3[0];
		q[1] = t1[1] + t2[1] + t3[1];
		q[2] = t1[2] + t2[2] + t3[2];
		q[3] = q1.q[3] * q2.q[3] -
			   (q1.q[0] * q2.q[0] + q1.q[1] * q2.q[1] + q1.q[2] * q2.q[2]);

		if (++count > RENORMCOUNT)
		{
			count = 0;
			normalize();
		}
	};

	Quat quatVectMult(const Vec3d vect)
	{
		Quat	ret;

		ret.q[3] = (double) (-(q[0] * vect[0] + q[1] * vect[1] + q[2] * vect[2]));
		ret.q[0] = (double) (q[3] * vect[0] + q[1] * vect[2] - q[2] * vect[1]);
		ret.q[1] = (double) (q[3] * vect[1] + q[2] * vect[0] - q[0] * vect[2]);
		ret.q[2] = (double) (q[3] * vect[2] + q[0] * vect[1] - q[1] * vect[0]);

		return ret;
	};

	Quat vectQuatMult(const Vec3d& vect)
	{
		Quat	ret;

		ret[3] = (double) (-(vect[0] * q[0] + vect[1] * q[1] + vect[2] * q[2]));
		ret[0] = (double) (vect[0] * q[3] + vect[1] * q[2] - vect[2] * q[1]);
		ret[1] = (double) (vect[1] * q[3] + vect[2] * q[0] - vect[0] * q[2]);
		ret[2] = (double) (vect[2] * q[3] + vect[0] * q[1] - vect[1] * q[0]);

		return ret;
	};

	Quat inverse() const
	{
		Quat	ret;

		double		norm	= sqrt(q[0] * q[0] +
								   q[1] * q[1] +
								   q[2] * q[2] +
								   q[3] * q[3]);

		if (norm != 0.0f)
		{
			norm = 1.0f / norm;
			ret.q[3] = q[3] * norm;
			for (int i = 0; i < 3; i++)
			{
				ret.q[i] = -q[i] * norm;
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				ret.q[i] = 0.0;
			}
		}
		return ret;
	};

	Vec3d toEulerVector() const
	{
		Quat q = *this;
		q.normalize();

		double angle = acos(q.q[3]) * 2;

		Vec3d v(q.q[0], q.q[1], q.q[2]);

		double norm = sqrt( (double) (v.x() * v.x() + v.y() * v.y() + v.z() * v.z()) );
		if (norm > 0.0005){
			v /= norm;
			v *= angle;
		}
		return v;
	};

	/*! Returns the slerp interpolation of Quaternions \p a and \p b, at time \p t.

         \p t should range in [0,1]. Result is \p a when \p t=0 and \p b when \p t=1.

         When \p allowFlip is \c true (default) the slerp interpolation will always use the "shortest path"
         between the Quaternions' orientations, by "flipping" the source Quaternion if needed (see
         negate()). */

	void slerp(const Quat& a, const Quat& b, float t, bool allowFlip)
	{
		float cosAngle =  (float)(a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3]);

		float c1, c2;
		
		// Linear interpolation for close orientations
		if ((1.0f - fabs(cosAngle)) < 0.01)
		{
			c1 = 1.0f - t;
			c2 = t;
		}
		else
		{
			// Spherical interpolation
			float angle    = acos(fabs(cosAngle));
			float sinAngle = sin(angle);
			c1 = sin(angle * (1.0f - t)) / sinAngle;
			c2 = sin(angle * t) / sinAngle;
		}

		// Use the shortest path
		if (allowFlip && (cosAngle < 0.0f))
		c1 = -c1;

		q[0] = c1*a[0] + c2*b[0];
		q[1] = c1*a[1] + c2*b[1];
		q[2] = c1*a[2] + c2*b[2];
		q[3] = c1*a[3] + c2*b[3];
	};

	Quat slerp(Quat &q1, double t)
	{
		Quat q0_1;
		for (unsigned int i = 0 ; i<3 ; i++)
			q0_1[i] = -q[i];

		q0_1[3] = q[3];

		q0_1 = q1 * q0_1;

		Vec3d axis, temp;
		double angle;

		q0_1.quatToAxis(axis, angle);

		temp = axis * sin(t * angle);
		for (unsigned int i = 0 ; i<3 ; i++)
			q0_1[i] = temp[i];

		q0_1[3] = cos(t * angle);
		q0_1 = q0_1 * (*this);
		return q0_1;
	};

	// Given an axis and angle, compute quaternion.
	Quat slerp2(Quat &q1, double t)
	{
		// quaternion to return
		Quat qm;

		// Calculate angle between them.
		double cosHalfTheta = q[3] * q1[3] + q[0] * q1[0] + q[1] * q1[1] + q[2] * q1[2];
		
		// if qa=qb or qa=-qb then theta = 0 and we can return qa
		if (fabs(cosHalfTheta) >= 1.0){
			qm[3] = q[3];qm[0] = q[0];qm[1] = q[1];qm[2] = q[2];
			return qm;
		}
		
		// Calculate temporary values.
		double halfTheta = acos(cosHalfTheta);
		double sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
		// if theta = 180 degrees then result is not fully defined
		// we could rotate around any axis normal to qa or qb
		if (fabs(sinHalfTheta) < 0.001){ // fabs is floating point absolute
			qm[3] = (double)(q[3] * 0.5 + q1[3] * 0.5);
			qm[0] = (double)(q[0] * 0.5 + q1[0] * 0.5);
			qm[1] = (double)(q[1] * 0.5 + q1[1] * 0.5);
			qm[2] = (double)(q[2] * 0.5 + q1[2] * 0.5);
			return qm;
		}
		double ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
		double ratioB = sin(t * halfTheta) / sinHalfTheta;
		
		//calculate Quaternion.
		qm[3] = (double)(q[3] * ratioA + q1[3] * ratioB);
		qm[0] = (double)(q[0] * ratioA + q1[0] * ratioB);
		qm[1] = (double)(q[1] * ratioA + q1[1] * ratioB);
		qm[2] = (double)(q[2] * ratioA + q1[2] * ratioB);
		return qm;
	};

	/// Build a rotation matrix, given a quaternion rotation.
	void buildRotationMatrix(double m[4][4]) const
	{
		m[0][0] = (1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]));
		m[0][1] = (2.0f * (q[0] * q[1] - q[2] * q[3]));
		m[0][2] = (2.0f * (q[2] * q[0] + q[1] * q[3]));
		m[0][3] = 0;

		m[1][0] = (2.0f * (q[0] * q[1] + q[2] * q[3]));
		m[1][1] = (1.0f - 2.0f * (q[2] * q[2] + q[0] * q[0]));
		m[1][2] = (2.0f * (q[1] * q[2] - q[0] * q[3]));
		m[1][3] = 0;

		m[2][0] = (2.0f * (q[2] * q[0] - q[1] * q[3]));
		m[2][1] = (2.0f * (q[1] * q[2] + q[0] * q[3]));
		m[2][2] = (1.0f - 2.0f * (q[1] * q[1] + q[0] * q[0]));
		m[2][3] = 0;

		m[3][0] = 0;
		m[3][1] = 0;
		m[3][2] = 0;
		m[3][3] = 1;
	};

	/// Given an axis and angle, compute quaternion.
	Quat axisToQuat(Vec3d a, double phi)
	{
		a = a / a.norm();
		q[0] = (double)a.x();
		q[1] = (double)a.y();
		q[2] = (double)a.z();

		q[0] = q[0] * (double)sin(phi / 2.0);
		q[1] = q[1] * (double)sin(phi / 2.0);
		q[2] = q[2] * (double)sin(phi / 2.0);

		q[3] = (double)cos(phi / 2.0);

		return *this;
	};

	/// Given a quaternion, compute an axis and angle
	void quatToAxis(Vec3d &a, double &phi)
	{
		const double  sine  = sin(acos(q[3]));

		if (!sine)
    		a = Vec3d(0.0,1.0,0.0);
		else
    		a = Vec3d(q[0],q[1],q[2])/ sine;

		phi =  (double) (acos(q[3]) * 2.0) ;
	};

	Quat createQuaterFromFrame(const Vec3d &lox, const Vec3d &loy, const Vec3d &loz)
	{
		Quat q;
		Mat3x3d m;

		for (unsigned int i=0 ; i<3 ;i++)
		{
			m[i][0] = lox[i];
			m[i][1] = loy[i];
			m[i][2] = loz[i];
		}
		q.fromMatrix(m);
		return q;
	};

	/// Create a quaternion from Euler
	static Quat createQuaterFromEuler(Vec3d v) 
	{
		double quat[4];      double a0 = v.elems[0];
		double a1 = v.elems[1];
		double a2 = v.elems[2];
		quat[3] = cos(a0/2)*cos(a1/2)*cos(a2/2) + sin(a0/2)*sin(a1/2)*sin(a2/2);
		quat[0] = sin(a0/2)*cos(a1/2)*cos(a2/2) - cos(a0/2)*sin(a1/2)*sin(a2/2);
		quat[1] = cos(a0/2)*sin(a1/2)*cos(a2/2) + sin(a0/2)*cos(a1/2)*sin(a2/2);
		quat[2] = cos(a0/2)*cos(a1/2)*sin(a2/2) - sin(a0/2)*sin(a1/2)*cos(a2/2);
		Quat quatResult( quat[0], quat[1], quat[2], quat[3] );
		return quatResult;
	};

	/// Return the quaternion resulting of the movement between 2 quaternions
	Quat quatDiff(Quat a, const Quat& b)
	{
	    // If the axes are not oriented in the same direction, flip the axis and angle of a to get the same convention than b
	    if (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3]<0)
	    {
	        a[0] = -a[0];
	        a[1] = -a[1];
	        a[2] = -a[2];
	        a[3] = -a[3];
	    }

	    Quat q = b.inverse() * a;
	    return q;
	};

	/// Return the eulerian vector resulting of the movement between 2 quaternions
	Vec3d angularDisplacement( Quat a, const Quat& b)
	{
		return quatDiff(a,b).toEulerVector();
	};

	bool operator==(const Quat& q) const
	{
		for (int i=0;i<4;i++)
			if ( fabs( q[i] - q.q[i] ) > EQUALITY_THRESHOLD ) return false;
		return true;
	};

	bool operator!=(const Quat& q) const
	{
		for (int i=0;i<4;i++)
			if ( fabs( q[i] - q.q[i] ) > EQUALITY_THRESHOLD ) return true;
		return false;
	};
};

#endif
