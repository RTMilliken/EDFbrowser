contains(QT_MAJOR_VERSION, 4) {

LIST = 0 1 2 3 4 5 6
for(a, LIST):contains(QT_MINOR_VERSION, $$a):error("you are using an old version of Qt")

contains(QT_MINOR_VERSION, 7) {
  LIST = 0
  for(a, LIST):contains(QT_PATCH_VERSION, $$a):error("you are using an old version of Qt")
}
}


TEMPLATE = app
TARGET = edfbrowser
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += qt
CONFIG += warn_on
CONFIG += release
CONFIG += static
CONFIG += largefile
QT += network

contains(QT_MAJOR_VERSION, 5) {
QT += widgets
QT += printsupport

win32 {
    QTPLUGIN += windowsprintersupport
} else:mac {
    QTPLUGIN += cocoaprintersupport
} else {
    QTPLUGIN += cupsprintersupport
}
}

OBJECTS_DIR = ./objects
MOC_DIR = ./moc

HEADERS += global.h
HEADERS += mainwindow.h
HEADERS += viewcurve.h
HEADERS += popup_messagewindow.h
HEADERS += check_edf_file.h
HEADERS += show_edf_hdr.h
HEADERS += signals_dialog.h
HEADERS += signal_chooser.h
HEADERS += ascii_export.h
HEADERS += edf_compat.h
HEADERS += colordialog.h
HEADERS += filter.h
HEADERS += filter_dialog.h
HEADERS += jump_dialog.h
HEADERS += about_dialog.h
HEADERS += edf_annotations.h
HEADERS += annotations_dock.h
HEADERS += options_dialog.h
HEADERS += nk2edf.h
HEADERS += xml.h
HEADERS += save_montage_dialog.h
HEADERS += load_montage_dialog.h
HEADERS += view_montage_dialog.h
HEADERS += show_actual_montage_dialog.h
HEADERS += print_to_edf.h
HEADERS += pagetime_dialog.h
HEADERS += print_to_bdf.h
HEADERS += ascii2edf.h
HEADERS += fino2edf.h
HEADERS += nexfin2edf.h
HEADERS += edfplusd_cnv.h
HEADERS += utc_date_time.h
HEADERS += emsa2edf.h
HEADERS += special_button.h
HEADERS += bdf2edf.h
HEADERS += edit_annotation_dock.h
HEADERS += popup_save_cancelwindow.h
HEADERS += save_annots.h
HEADERS += filtercurve.h
HEADERS += utils.h
HEADERS += fourier.h
HEADERS += signalcurve.h
HEADERS += spectrumanalyzer.h
HEADERS += bi9800.h
HEADERS += edflib.h
HEADERS += export_annotations.h
HEADERS += edit_predefined_mtg.h
HEADERS += spectrum_dock.h
HEADERS += edf_annot_list.h
HEADERS += reduce_signals.h
HEADERS += active_file_chooser.h
HEADERS += header_editor.h
HEADERS += biosemi2bdfplus.h
HEADERS += bdf_triggers.h
HEADERS += adjustfiltersettings.h
HEADERS += import_annotations.h
HEADERS += ravg_filter.h
HEADERS += wav2edf.h
HEADERS += averager_dialog.h
HEADERS += averager_curve_wnd.h
HEADERS += ecg_filter.h
HEADERS += ecg_export.h
HEADERS += statistics_dialog.h
HEADERS += filteredblockread.h
HEADERS += flywheel.h
HEADERS += z_score_dialog.h
HEADERS += z_ratio_filter.h
HEADERS += raw2edf.h
HEADERS += check_for_updates.h
HEADERS += manscan2edf.h
HEADERS += scp_ecg2edf.h
HEADERS += unisens2edf.h

HEADERS += third_party/fidlib/fidlib.h
HEADERS += third_party/fidlib/fidmkf.h
HEADERS += third_party/fidlib/fidrf_cmdlist.h

HEADERS += third_party/kiss_fft/kiss_fft.h
HEADERS += third_party/kiss_fft/_kiss_fft_guts.h
HEADERS += third_party/kiss_fft/kiss_fftr.h


SOURCES += main.cpp
SOURCES += mainwindow.cpp
SOURCES += viewcurve.cpp
SOURCES += popup_messagewindow.cpp
SOURCES += check_edf_file.cpp
SOURCES += show_edf_hdr.cpp
SOURCES += signals_dialog.cpp
SOURCES += signal_chooser.cpp
SOURCES += ascii_export.cpp
SOURCES += edf_compat.cpp
SOURCES += colordialog.cpp
SOURCES += filter.cpp
SOURCES += filter_dialog.cpp
SOURCES += jump_dialog.cpp
SOURCES += about_dialog.cpp
SOURCES += edf_annotations.cpp
SOURCES += annotations_dock.cpp
SOURCES += options_dialog.cpp
SOURCES += nk2edf.cpp
SOURCES += xml.cpp
SOURCES += save_montage_dialog.cpp
SOURCES += load_montage_dialog.cpp
SOURCES += view_montage_dialog.cpp
SOURCES += show_actual_montage_dialog.cpp
SOURCES += print_to_edf.cpp
SOURCES += pagetime_dialog.cpp
SOURCES += print_to_bdf.cpp
SOURCES += ascii2edf.cpp
SOURCES += fino2edf.cpp
SOURCES += nexfin2edf.cpp
SOURCES += edfplusd_cnv.cpp
SOURCES += utc_date_time.c
SOURCES += emsa2edf.cpp
SOURCES += special_button.cpp
SOURCES += bdf2edf.cpp
SOURCES += edit_annotation_dock.cpp
SOURCES += popup_save_cancelwindow.cpp
SOURCES += save_annots.cpp
SOURCES += filtercurve.cpp
SOURCES += utils.c
SOURCES += fourier.c
SOURCES += signalcurve.cpp
SOURCES += spectrumanalyzer.cpp
SOURCES += bi9800.cpp
SOURCES += edflib.c
SOURCES += export_annotations.cpp
SOURCES += edit_predefined_mtg.cpp
SOURCES += spectrum_dock.cpp
SOURCES += edf_annot_list.c
SOURCES += reduce_signals.cpp
SOURCES += active_file_chooser.cpp
SOURCES += header_editor.cpp
SOURCES += biosemi2bdfplus.cpp
SOURCES += bdf_triggers.cpp
SOURCES += adjustfiltersettings.cpp
SOURCES += import_annotations.cpp
SOURCES += ravg_filter.cpp
SOURCES += wav2edf.cpp
SOURCES += averager_dialog.cpp
SOURCES += averager_curve_wnd.cpp
SOURCES += ecg_filter.cpp
SOURCES += ecg_export.cpp
SOURCES += statistics_dialog.cpp
SOURCES += filteredblockread.cpp
SOURCES += flywheel.cpp
SOURCES += z_score_dialog.cpp
SOURCES += z_ratio_filter.cpp
SOURCES += raw2edf.cpp
SOURCES += check_for_updates.cpp
SOURCES += manscan2edf.cpp
SOURCES += scp_ecg2edf.cpp
SOURCES += unisens2edf.cpp

SOURCES += third_party/fidlib/fidlib.c

SOURCES += third_party/kiss_fft/kiss_fft.c
SOURCES += third_party/kiss_fft/kiss_fftr.c


RESOURCES = images.qrc

win32 {
 RC_FILE = edfbrowser.rc
}


