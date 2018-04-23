#ifndef MEDIAPREPARERCONTROLLER_HPP
#define MEDIAPREPARERCONTROLLER_HPP

#include <MediaPreparerGUI.hpp>

#include <QApplication>

namespace SuperEpicFuntime {

class MediaPreparerController {
  private:
	MediaPreparerGUI *p = new MediaPreparerGUI();
	Ui::MediaPreparer *ui = p->ui;

  public:
	MediaPreparerController();
	MediaPreparerGUI *getParent();
	Ui::MediaPreparer *getUI();

	void show();
};
} // namespace SuperEpicFuntime
#endif // MEDIAPREPARERCONTROLLER_HPP
