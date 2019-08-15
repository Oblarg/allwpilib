package edu.wpi.first.wpilibj.spline;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import org.ejml.simple.SimpleMatrix;

public class QuinticHermiteSpline extends Spline {

  private SimpleMatrix m_coefficients;
  private static SimpleMatrix hermiteBasis = null;

  public QuinticHermiteSpline(Pose2d start, Pose2d end) {
    super(5);

    // Populate the coefficients for the actual spline equations.
    // Row 0 is x coefficients
    // Row 1 is y coefficients
    final var hermite = makeHermiteBasis();
    final var controlVectors = makeControlVectors(start, end);

    final var x = controlVectors[0];
    final var y = controlVectors[1];

    final var xCoeffs = (hermite.mult(x)).transpose();
    final var yCoeffs = (hermite.mult(y)).transpose();

    m_coefficients = new SimpleMatrix(6, 6);

    for (int i = 0; i < 6; i++) {
      m_coefficients.set(0, i, xCoeffs.get(0, i));
      m_coefficients.set(1, i, yCoeffs.get(0, i));

      // Populate Row 2 and Row 3 with the derivatives of the equations above.
      // Then populate row 4 and 5 with the second derivatives.
      m_coefficients.set(2, i, m_coefficients.get(0, i) * (5 - i));
      m_coefficients.set(3, i, m_coefficients.get(1, i) * (5 - i));
      m_coefficients.set(4, i, m_coefficients.get(2, i) * (5 - i));
      m_coefficients.set(5, i, m_coefficients.get(3, i) * (5 - i));
    }
  }

  @Override
  protected SimpleMatrix getCoefficients() {
    return m_coefficients;
  }

  /**
   * Returns the hermite basis matrix for quintic hermite spline interpolation.
   *
   * @return The hermite basis matrix for quintic hermite spline interpolation.
   */
  private SimpleMatrix makeHermiteBasis() {
    if (hermiteBasis == null) {
      hermiteBasis = new SimpleMatrix(6, 6, true, new double[]{
          -06.0, -03.0, -00.5, +00.5, -03.0, +06.0,
          +15.0, +08.0, +01.5, -01.0, +07.0, -15.0,
          -10.0, -06.0, -01.5, +00.5, -04.0, +10.0,
          +00.0, +00.0, +00.5, +00.0, +00.0, +00.0,
          +00.0, +01.0, +00.0, +00.0, +00.0, +00.0,
          +01.0, +00.0, +00.0, +00.0, +00.0, +00.0
      });
    }
    return hermiteBasis;
  }

  /**
   * Converts the constructor arguments to the control vectors to calculate the
   * spline coefficients.
   *
   * @param start The starting position for this spline
   * @param end   The end position for this spline
   * @return Control vectors.
   */
  @SuppressWarnings("LocalVariableName")
  private SimpleMatrix[] makeControlVectors(Pose2d start, Pose2d end) {
    // This just makes the splines look better.
    double scalar = start.getTranslation().getDistance(end.getTranslation()) * 1.2;

    var xControlVector = new SimpleMatrix(6, 1, true, new double[]{
        start.getTranslation().getX(), start.getRotation().getCos() * scalar,
        0.0, 0.0,
        end.getRotation().getCos() * scalar, end.getTranslation().getX()
    });

    var yControlVector = new SimpleMatrix(6, 1, true, new double[]{
        start.getTranslation().getY(), start.getRotation().getSin() * scalar,
        0.0, 0.0,
        end.getRotation().getSin() * scalar, end.getTranslation().getY()
    });

    return new SimpleMatrix[]{xControlVector, yControlVector};
  }
}
