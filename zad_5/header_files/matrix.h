template <typename T> class Matrix
{
 static const int size = 3;
 std::array<std::array<T, size>, size> cf;
 void setIdentity();
 void copyMx(const Matrix& rhm);
public:
 Matrix() { setIdentity(); }
 Matrix(const Matrix& rhm) { copyMx(rhm); }
 Matrix& operator = (const Matrix& rhm) { copyMx(rhm); return *this; }
 static Matrix scaleMx(T scaleX, T scaleY);
 static Matrix translateMx(T offsetX, T offsetY);
 static Matrix rotateMx(T angle);
 friend std::ostream& operator <<(ostream& os, const Matrix& mx);
 friend Matrix operator * (const Matrix& lhm, const Matrix& rhm);
 Matrix& operator *= (const Matrix& rhm);
 FPoint transform(const FPoint& pt) const;
 const T& operator(unsigned int rowid, unsigned int colid) const;
 T& operator(unsigned int rowid, unsigned int colid);
};