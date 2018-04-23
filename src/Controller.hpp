#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <EventHandler.hpp>
#include <MediaPreparer.hpp>
#include <QtWidgets/QWidget>

#include <iostream> // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
class Controller {
  private:
	QWidget *widget;

  public:
	Controller(QWidget *widget);
	QWidget *getWidget();

	void show();
};

} // namespace SuperEpicFuntime
#endif // CONTROLLER_HPP
