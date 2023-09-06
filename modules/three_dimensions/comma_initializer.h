#pragma once

/**
 * Eigen/src/Core/CommaInitializer.h
 *
 * \class CommaInitializer
 * \ingroup Core_Module
 *
 * \brief Helper class used by the comma initializer operator
 *
 * This class is internally used to implement the comma initializer feature. It is
 * the return type of MatrixBase::operator<<, and most of the time this is the only
 * way it is used.
 *
 * \sa \blank \ref MatrixBaseCommaInitRef "MatrixBase::operator<<", CommaInitializer::finished()
 */
template <typename XprType>
struct CommaInitializer {
    typedef typename XprType::Scalar Scalar;

    /* inserts a scalar value in the target matrix */
    CommaInitializer& operator,(const Scalar& s) {
        if (m_col == m_xpr.cols()) {
            m_row += m_currentBlockRows;
            m_col = 0;
            m_currentBlockRows = 1;
            eigen_assert(m_row < m_xpr.rows() && "Too many rows passed to comma initializer (operator<<)");
        }
        eigen_assert(m_col < m_xpr.cols() && "Too many coefficients passed to comma initializer (operator<<)");
        eigen_assert(m_currentBlockRows == 1);
        m_xpr.coeffRef(m_row, m_col++) = s;
        return *this;
    }

    /* inserts a matrix expression in the target matrix */
    template <typename OtherDerived>
    CommaInitializer& operator,(const DenseBase<OtherDerived>& other) {
        if (m_col == m_xpr.cols() && (other.cols() != 0 || other.rows() != m_currentBlockRows)) {
            m_row += m_currentBlockRows;
            m_col = 0;
            m_currentBlockRows = other.rows();
            eigen_assert(m_row + m_currentBlockRows <= m_xpr.rows() &&
                         "Too many rows passed to comma initializer (operator<<)");
        }
        eigen_assert((m_col + other.cols() <= m_xpr.cols()) &&
                     "Too many coefficients passed to comma initializer (operator<<)");
        eigen_assert(m_currentBlockRows == other.rows());
        m_xpr.template block<OtherDerived::RowsAtCompileTime, OtherDerived::ColsAtCompileTime>(
          m_row, m_col, other.rows(), other.cols()) = other;
        m_col += other.cols();
        return *this;
    }

    EIGEN_DEVICE_FUNC
    inline ~CommaInitializer() { finished(); }
};

// $ vi /media/zhishan/chitu-3/zhi3/Documents/Eigen/Examples/c++/Eigen/Matrix/Matrix.cpp