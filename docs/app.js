const questions = [
    ['1a', 'Créer un brin', 'Initialise les trois relations sur le nouveau brin.'],
    ['1b', 'Appliquer les relations alpha', 'Calcule une image, une composition et le statut libre d’un brin.'],
    ['1c', 'Lier deux brins', 'Construis une liaison symétrique entre deux brins libres.'],
    ['1d', 'Vérifier la validité', 'Contrôle les involutions et les points fixes.'],
    ['2a', 'Calculer une orbite', 'Visite les brins accessibles par les relations demandées.'],
    ['2b', 'Parcourir une orbite ordonnée', 'Applique cycliquement la suite de relations jusqu’au retour au départ.'],
    ['2c', 'Compter les cellules', 'Vérifie les sommets, les arêtes et les faces du carré.', 'Fonction elements fournie dans le TP.'],
    ['3', 'Plonger les sommets', 'Retrouve le brin qui porte la propriété géométrique d’un sommet.'],
    ['4', 'Coudre deux cellules', 'Apparie les brins de la couture, puis teste le cube et le cube troué.'],
    ['5', 'Calculer la caractéristique d’Euler', 'Déduis S − A + F du nombre de cellules de chaque degré.'],
    ['6', 'Visualiser le maillage', 'Observe le cube et le cube troué dans le navigateur.', 'Visualiseur et extraction fournis dans le TP.'],
    ['7', 'Construire le dual', 'Échange les relations de degré 0 et 2 et place les nouveaux sommets.'],
    ['8', 'Comparer les représentations', 'Compare la G-carte à une 2-carte, une structure Half-Edge et une liste de faces.', 'Question de discussion.'],
];

const boxes = new Map();
for (const [id, title, instruction, note] of questions) {
    const card = document.createElement('article');
    const heading = document.createElement('h3');
    const number = document.createElement('span');
    number.className = 'question-id';
    number.textContent = id;
    heading.append(number, title);
    const prompt = document.createElement('p');
    prompt.textContent = instruction;
    const solution = document.createElement('div');
    solution.className = 'solution';
    solution.textContent = note || 'Corrigé à venir';
    if (!note) boxes.set(id, solution);
    card.append(heading, prompt, solution);
    document.querySelector('#questions').append(card);
}

const refresh = document.querySelector('#refresh');
const status = document.querySelector('#status');

async function loadSolutions() {
    refresh.disabled = true;
    try {
        const config = await fetch('config.json', {cache: 'no-store'});
        if (!config.ok) throw new Error('Configuration indisponible.');
        const {apiBase} = await config.json();
        if (!apiBase) {
            status.textContent = 'Le service de publication des corrigés est en cours de configuration.';
            return;
        }
        const response = await fetch(new URL('/api/solutions', apiBase), {cache: 'no-store'});
        if (!response.ok) throw new Error('Service indisponible.');
        const {solutions} = await response.json();
        for (const {id, code} of solutions) {
            const box = boxes.get(id);
            if (!box || typeof code !== 'string') continue;
            const details = document.createElement('details');
            details.open = box.querySelector('details')?.open ?? false;
            const summary = document.createElement('summary');
            summary.textContent = 'Voir le corrigé';
            const pre = document.createElement('pre');
            const cpp = document.createElement('code');
            cpp.textContent = code;
            pre.append(cpp);
            details.append(summary, pre);
            box.replaceChildren(details);
        }
        status.textContent = '';
    } catch {
        status.textContent = 'Impossible d’actualiser les corrigés. Réessaie dans un instant.';
    } finally {
        refresh.disabled = false;
    }
}
refresh.addEventListener('click', loadSolutions);
loadSolutions();
