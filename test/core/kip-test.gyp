{
  'targets': [
    {
      'target_name': 'kip-test',
      'type': 'executable',
      'msvs_guid': '5ECEC9E5-8F23-47B6-93E0-C3B328B3BE65',
      'include_dirs': [
        '../include',
        '../third_party/catch',
        '../third_party/boost/include',
      ],
      'sources': [
        'main.cpp',
        'test-print-schema-document.cpp',
      ],
      'conditions': [
      ],
    },
  ],
}
