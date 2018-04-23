#ifndef MEDIAPREPARERCONTROLLER_HPP
#define MEDIAPREPARERCONTROLLER_HPP

#include <QtWidgets/QWidget>

#include <iostream> // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
class MediaPreparerController {
  private:
	QWidget *widget;

  public:
	MediaPreparerController(QWidget *widget);
	QWidget *getWidget();

	void show();
};

} // namespace SuperEpicFuntime
#endif // MEDIAPREPARERCONTROLLER_HPP
