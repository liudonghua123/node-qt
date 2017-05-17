{
  'targets': [
    {
      'target_name': 'qt',
      'sources': [
        'src/qt.cc', 

        'src/QtCore/qsize.cc',
        'src/QtCore/qpointf.cc',

        'src/QtGui/qapplication.cc',
        'src/QtGui/qwidgetwrapbase.cc',
        'src/QtGui/qwidget.cc',
        'src/QtGui/qmouseevent.cc',
        'src/QtGui/qkeyevent.cc',
        'src/QtGui/qpixmap.cc',
        'src/QtGui/qpainter.cc',
        'src/QtGui/qcolor.cc',
        'src/QtGui/qbrush.cc',
        'src/QtGui/qpen.cc',
        'src/QtGui/qimage.cc',
        'src/QtGui/qpainterpath.cc',
        'src/QtGui/qfont.cc',
        'src/QtGui/qmatrix.cc',
        'src/QtGui/qsound.cc',
        'src/QtGui/qscrollarea.cc',
        'src/QtGui/qscrollbar.cc',
        'src/QtGui/qpushbutton.cc',
        'src/QtGui/qlabel.cc',
        'src/QtGui/qboxlayout.cc',

        'src/QtTest/qtesteventlist.cc'
      ],
      'include_dirs' : [
        '<!(node -e "require(\'nan\')")'
      ],
      'conditions': [
        ['OS=="mac"', { # mac with qt from homebrew
          'include_dirs': [
            '<!@(pkg-config --cflags Qt5Core Qt5Gui Qt5Test Qt5Widgets Qt5Multimedia|sed "s/-D[A-z_]*//g; s/-I//g")',
          ],
          'libraries': [
            '<!@(pkg-config --libs Qt5Core Qt5Gui Qt5Test Qt5Widgets Qt5Multimedia|sed \'s,-F[^\ ]*,,g; s, -framework \([A-z_]*\),/\\1.framework,g;\')',
            'QtCore.framework',
          ],
          'mac_framework_dirs': [
            '<!@(pkg-config --libs Qt5Core Qt5Gui Qt5Test Qt5Widgets Qt5Multimedia|sed \'s, -framework [A-z_]*,,g; s,-F,,g\')',
          ],
        }],
        ['OS=="linux"', {
          'cflags': [
            '<!@(pkg-config --cflags QtCore QtGui QtTest)'
          ],
          'ldflags': [
            '<!@(pkg-config --libs-only-L --libs-only-other QtCore QtGui QtTest)'
          ],
          'libraries': [
            '<!@(pkg-config --libs-only-l QtCore QtGui QtTest)'
          ]
        }],
        ['OS=="win"', {
          'include_dirs': [
              'deps/qt-4.8.0/win32/ia32/include',
              'deps/qt-4.8.0/win32/ia32/include/QtCore',
              'deps/qt-4.8.0/win32/ia32/include/QtGui',
              'deps/qt-4.8.0/win32/ia32/include/QtTest',
          ],
          'libraries': [
              # TODO: fix node-gyp behavior that requires ../
              '../deps/qt-4.8.0/win32/ia32/lib/QtCore4.lib',
              '../deps/qt-4.8.0/win32/ia32/lib/QtGui4.lib',
              '../deps/qt-4.8.0/win32/ia32/lib/QtTest4.lib'
          ]
        }]        
      ],
      'configurations': {
        'Release': {
          'conditions': [
            ['OS=="mac"', {
              'xcode_settings': {
                'GCC_GENERATE_DEBUGGING_SYMBOLS': 'NO'
              }
            }]
          ]
        }
      }
    }
  ]
}
