#ifndef FractureMarkerBase_H
#define FractureMarkerBase_H

#include "Marker.h"
#include "Coupleable.h"
#include "MaterialPropertyInterface.h"

class FractureMarkerBase;

template <>
InputParameters validParams<FractureMarkerBase>();

class FractureMarkerBase : public Marker, public MaterialPropertyInterface
{
public:
  FractureMarkerBase(const InputParameters & parameters);

protected:
  virtual MarkerValue computeElementMarker() override;

  /**
   * Override this to compute a marker value at each quadrature point.
   *
   * The ultimate value for the element will be taken as the maximum
   * (most conservative) value for all quadrature points on the element.
   *
   * @return The MarkerValue at one quadrature point.
   */
  virtual MarkerValue computeQpMarker() = 0;

  /// The quadrature rule for the system
  QBase *& _qrule;

  /// Position of the current quadrature point
  const MooseArray<Point> & _q_point;

  /// The current quadrature point
  unsigned int _qp;
};

#endif /* FractureMarkerBase_H */
