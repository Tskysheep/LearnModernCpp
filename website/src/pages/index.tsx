import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import Translate, {translate} from '@docusaurus/Translate';
import styles from './index.module.css';

type FeatureItem = {
  title: string;
  description: string;
  link: string;
  emoji: string;
};

const featureList: FeatureItem[] = [
  {
    title: 'C++11',
    description: translate({
      id: 'homepage.feature.cpp11',
      message: 'auto、lambda、右值引用、智能指针、可变参数模板等现代 C++ 基石特性',
    }),
    link: '/docs/cpp11',
    emoji: '🚀',
  },
  {
    title: 'C++14',
    description: translate({
      id: 'homepage.feature.cpp14',
      message: '泛型 lambda、变量模板、std::make_unique、放宽的 constexpr 等改进',
    }),
    link: '/docs/cpp14',
    emoji: '⚡',
  },
  {
    title: 'C++17',
    description: translate({
      id: 'homepage.feature.cpp17',
      message: '结构化绑定、if constexpr、std::optional、std::variant、折叠表达式等',
    }),
    link: '/docs/cpp17',
    emoji: '🔧',
  },
  {
    title: 'C++20',
    description: translate({
      id: 'homepage.feature.cpp20',
      message: 'Concepts、Ranges、Coroutines、Modules、三路比较运算符等重大更新',
    }),
    link: '/docs/cpp20',
    emoji: '🌟',
  },
  {
    title: 'C++23',
    description: translate({
      id: 'homepage.feature.cpp23',
      message: 'std::expected、std::print、推导 this、多维下标运算符等最新特性',
    }),
    link: '/docs/cpp23',
    emoji: '🔮',
  },
];

function HeroBanner() {
  const {siteConfig} = useDocusaurusContext();
  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)}>
      <div className="container">
        <h1 className={styles.heroTitle}>{siteConfig.title}</h1>
        <p className={styles.heroSubtitle}>{siteConfig.tagline}</p>
        <div className={styles.buttons}>
          <Link
            className="button button--secondary button--lg"
            to="/docs/intro">
            <Translate id="homepage.callToAction">快速开始 →</Translate>
          </Link>
        </div>
      </div>
    </header>
  );
}

function Feature({title, description, link, emoji}: FeatureItem) {
  return (
    <div className={clsx('col col--4', styles.featureCol)}>
      <Link to={link} className={styles.featureCard}>
        <div className={styles.featureEmoji}>{emoji}</div>
        <h3 className={styles.featureTitle}>{title}</h3>
        <p className={styles.featureDescription}>{description}</p>
      </Link>
    </div>
  );
}

function FeaturesSection() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {featureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}

export default function Home(): JSX.Element {
  const {siteConfig} = useDocusaurusContext();
  return (
    <Layout
      title={siteConfig.title}
      description={siteConfig.tagline}>
      <HeroBanner />
      <main>
        <FeaturesSection />
      </main>
    </Layout>
  );
}
