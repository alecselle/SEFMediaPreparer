#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <Event.hpp>
#include <Library.hpp>
#include <MediaPreparer.hpp>
#include <QtWidgets/QWidget>
#include <Settings.hpp>

#include <iostream> // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
class Controller {
  private:
	MediaPreparer *mediaPreparer;

  public:
	Controller();

	void show();
};

} // namespace SuperEpicFuntime
#endif // CONTROLLER_HPP
