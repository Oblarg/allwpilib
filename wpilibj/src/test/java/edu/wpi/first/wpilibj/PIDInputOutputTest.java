package edu.wpi.first.wpilibj;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import edu.wpi.first.wpilibj.experimental.controller.PIDController;

import static org.junit.jupiter.api.Assertions.assertEquals;

class PIDInputOutputTest {
  private PIDController m_controller;

  @BeforeEach
  void setUp() {
    m_controller = new PIDController(0, 0, 0);
  }

  @Test
  void proportionalGainOutputTest() {
    m_controller.setP(4);

    double out = m_controller.calculate(2.5, 0);

    assertEquals(out, -10., 1e-5);
  }

  @Test
  void integralGainOutputTest() {
    m_controller.setI(4);

    double out = 0;

    for (int i = 0; i < 10; i++) {
      out = m_controller.calculate(2.5, 0);
    }

    assertEquals(out, -100.*m_controller.getPeriod(), 1e-5);
  }

  @Test
  void derivativeGainOutputTest() {
    m_controller.setI(4);

    m_controller.calculate(0, 0);
    double out = m_controller.calculate(2.5, 0);

    assertEquals(out, -10./m_controller.getPeriod());
  }
}