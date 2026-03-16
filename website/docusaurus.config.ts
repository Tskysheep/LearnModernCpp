import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';

const config: Config = {
  title: 'LearnModernCpp',
  tagline: '现代 C++ 完整教学示例 — 从 C++11 到 C++23',
  favicon: 'img/logo.svg',

  url: 'https://tskysheep.github.io',
  baseUrl: '/LearnModernCpp/',

  organizationName: 'Tskysheep',
  projectName: 'LearnModernCpp',

  trailingSlash: true,
  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',

  i18n: {
    defaultLocale: 'zh-Hans',
    locales: ['zh-Hans'],
  },

  markdown: {
    mermaid: true,
  },

  themes: ['@docusaurus/theme-mermaid'],

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.ts',
          showLastUpdateTime: false,
          breadcrumbs: true,
        },
        blog: false,
        theme: {
          customCss: './src/css/custom.css',
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    navbar: {
      title: 'LearnCpp',
      logo: {
        alt: 'LearnCpp Logo',
        src: 'img/logo.svg',
      },
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'docs',
          position: 'left',
          label: '文档',
        },
        {
          href: 'https://github.com/learncpp/learncpp',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: '文档',
          items: [
            {label: '快速开始', to: '/docs/intro'},
            {label: 'C++11', to: '/docs/cpp11'},
            {label: 'C++14', to: '/docs/cpp14'},
            {label: 'C++17', to: '/docs/cpp17'},
            {label: 'C++20', to: '/docs/cpp20'},
            {label: 'C++23', to: '/docs/cpp23'},
          ],
        },
        {
          title: '参考',
          items: [
            {label: 'cppreference', href: 'https://en.cppreference.com/'},
            {label: 'C++ 标准', href: 'https://isocpp.org/'},
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} LearnCpp. Built with Docusaurus.`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
      additionalLanguages: ['bash', 'json', 'cpp', 'cmake'],
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
