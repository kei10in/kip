{
    'target_defaults': {
        'configurations': {
            'Debug_Win32': {
                'msvs_configuration_platform': 'Win32',
                'msbuild_settings': {
                    'ClCompile': {
                        'Optimization': 'Disabled',
                        'RuntimeLibrary': 'MultiThreadedDebugDLL',
                    },
                },
            },
            'Debug_x64': {
                'inherit_from': [ 'Debug_Win32' ],
                'msvs_configuration_platform': 'x64',
            },
            'Release_Win32': {
                'defines': [ 'NDEBUG' ],
                'msvs_configuration_platform': 'Win32',
            },
            'Release_x64': {
                'inherit_from': [ 'Release_Win32' ],
                'msvs_configuration_platform': 'x64',
            },
        },
        'include_dirs': [
            'include',
            'third_party/catch',
        ],
        'sources': [
            'include/kip/elements.hpp',
            'include/kip/windows/dom-traits.hpp',
            'include/kip/windows/utils.hpp',
            'include/kip/windows/utils.hpp',
            'include/kip/windows/defs.hpp',
            'include/kip/windows/dump.hpp',
            'include/kip/windows/parser.hpp',
            'include/kip/xml/dom-traits.hpp',
            'include/kip/xml/qname.hpp',
            'include/kip/xml/utils.hpp',
            'include/kip/framework.hpp',
            'include/kip/hash-combine.hpp',
            'include/kip/keywords.hpp',
            'include/kip/print-schema.hpp',
            'include/kip/xml-ns.hpp',
        ],
        'msbuild_configuration_attributes': {
            'CharacterSet': 'Unicode'
        }
    },
    'targets': [
        {
            'target_name': 'kip-test',
            'type': 'executable',
            'msvs_guid': '5ECEC9E5-8F23-47B6-93E0-C3B328B3BE65',
            'sources': [
                'test/core/main.cpp',
                'test/core/test-print-schema-document.cpp',
            ],
        },
    ],
    'conditions': [
        ['OS=="win"', {
            'targets': [
                {
                    'target_name': 'kip-test-windows',
                    'type': 'executable',
                    'msvs_guid': '5ECEC9E5-8F23-47B6-93E0-C3B328B3BE66',
                    'sources': [
                        'test/windows/main.cpp',
                        'test/windows/test-print-schema-document.cpp',
                        'test/windows/test-hoge.cpp'
                    ],
                    'link_settings': {
                        'libraries': [
                            'Ole32.lib',
                            'Prntvpt.lib',
                            'Shlwapi.lib',
                            'XmlLite.lib',
                        ],
                    },
                    'msbuild_settings': {
                        'Link': {
                            'SubSystem': 'Console',
                        },
                    },
                },
            ],
        }],
    ],
}
