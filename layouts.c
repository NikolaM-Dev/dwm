// -------------------------------- Bstack ----------------------------------

// https : //dwm.suckless.org/patches/bottomstack/
void bstack(Monitor *m)
{
	int w, h, mh, mx, tx, ty, tw;
	unsigned int i, n;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
		;
	if (n == 0)
		return;
	if (n > m->nmaster)
	{
		mh = m->nmaster ? m->mfact * m->wh : 0;
		tw = m->ww / (n - m->nmaster);
		ty = m->wy + mh;
	}
	else
	{
		mh = m->wh;
		tw = m->ww;
		ty = m->wy;
	}
	for (i = mx = 0, tx = m->wx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
	{
		if (i < m->nmaster)
		{
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx + mx, m->wy, w - (2 * c->bw), mh - (2 * c->bw), 0);
			mx += WIDTH(c);
		}
		else
		{
			h = m->wh - mh;
			resize(c, tx, ty, tw - (2 * c->bw), h - (2 * c->bw), 0);
			if (tw != m->ww)
				tx += WIDTH(c);
		}
	}
}

// -------------------------------- Fibonacci ----------------------------------

// https://dwm.suckless.org/patches/fibonacci/
void fibonacci(Monitor *mon, int s)
{
	unsigned int i, n, nx, ny, nw, nh;
	Client *c;

	for (n = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next), n++)
		;
	if (n == 0)
		return;

	nx = mon->wx;
	ny = 0;
	nw = mon->ww;
	nh = mon->wh;

	for (i = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next))
	{
		if ((i % 2 && nh / 2 > 2 * c->bw) || (!(i % 2) && nw / 2 > 2 * c->bw))
		{
			if (i < n - 1)
			{
				if (i % 2)
					nh /= 2;
				else
					nw /= 2;
				if ((i % 4) == 2 && !s)
					nx += nw;
				else if ((i % 4) == 3 && !s)
					ny += nh;
			}
			if ((i % 4) == 0)
			{
				if (s)
					ny += nh;
				else
					ny -= nh;
			}
			else if ((i % 4) == 1)
				nx += nw;
			else if ((i % 4) == 2)
				ny += nh;
			else if ((i % 4) == 3)
			{
				if (s)
					nx += nw;
				else
					nx -= nw;
			}
			if (i == 0)
			{
				if (n != 1)
					nw = mon->ww * mon->mfact;
				ny = mon->wy;
			}
			else if (i == 1)
				nw = mon->ww - nw;
			i++;
		}
		resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, False);
	}
}

void dwindle(Monitor *mon)
{
	fibonacci(mon, 1);
}

void spiral(Monitor *mon)
{
	fibonacci(mon, 0);
}

// ------------------------------- Double Deck --------------------------------

// https://dwm.suckless.org/patches/deck/
void doubledeck(Monitor *m)
{
	unsigned int i, n, mw;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
		;
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? m->ww * m->mfact : 0;
	else
		mw = m->ww;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster)
			resize(c, m->wx, m->wy, mw - (2 * c->bw), m->wh - (2 * c->bw), False);
		else
			resize(c, m->wx + mw, m->wy, m->ww - mw - (2 * c->bw), m->wh - (2 * c->bw), False);
}

// ------------------------------ Centered Master -----------------------------

// https://dwm.suckless.org/patches/centeredmaster/
void centeredmaster(Monitor *m)
{
	unsigned int i, n, h, mw, mx, my, oty, ety, tw;
	Client *c;

	/* count number of clients in the selected monitor */
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
		;
	if (n == 0)
		return;

	/* initialize areas */
	mw = m->ww;
	mx = 0;
	my = 0;
	tw = mw;

	if (n > m->nmaster)
	{
		/* go mfact box in the center if more than nmaster clients */
		mw = m->nmaster ? m->ww * m->mfact : 0;
		tw = m->ww - mw;

		if (n - m->nmaster > 1)
		{
			/* only one client */
			mx = (m->ww - mw) / 2;
			tw = (m->ww - mw) / 2;
		}
	}

	oty = 0;
	ety = 0;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster)
		{
			/* nmaster clients are stacked vertically, in the center
		 * of the screen */
			h = (m->wh - my) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx + mx, m->wy + my, mw - (2 * c->bw),
				   h - (2 * c->bw), 0);
			my += HEIGHT(c);
		}
		else
		{
			/* stack clients are stacked vertically */
			if ((i - m->nmaster) % 2)
			{
				h = (m->wh - ety) / ((1 + n - i) / 2);
				resize(c, m->wx, m->wy + ety, tw - (2 * c->bw),
					   h - (2 * c->bw), 0);
				ety += HEIGHT(c);
			}
			else
			{
				h = (m->wh - oty) / ((1 + n - i) / 2);
				resize(c, m->wx + mx + mw, m->wy + oty,
					   tw - (2 * c->bw), h - (2 * c->bw), 0);
				oty += HEIGHT(c);
			}
		}
}

void centeredfloatingmaster(Monitor *m)
{
	unsigned int i, n, w, mh, mw, mx, mxo, my, myo, tx;
	Client *c;

	/* count number of clients in the selected monitor */
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
		;
	if (n == 0)
		return;

	/* initialize nmaster area */
	if (n > m->nmaster)
	{
		/* go mfact box in the center if more than nmaster clients */
		if (m->ww > m->wh)
		{
			mw = m->nmaster ? m->ww * m->mfact : 0;
			mh = m->nmaster ? m->wh * 0.9 : 0;
		}
		else
		{
			mh = m->nmaster ? m->wh * m->mfact : 0;
			mw = m->nmaster ? m->ww * 0.9 : 0;
		}
		mx = mxo = (m->ww - mw) / 2;
		my = myo = (m->wh - mh) / 2;
	}
	else
	{
		/* go fullscreen if all clients are in the master area */
		mh = m->wh;
		mw = m->ww;
		mx = mxo = 0;
		my = myo = 0;
	}

	for (i = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster)
		{
			/* nmaster clients are stacked horizontally, in the center
		 * of the screen */
			w = (mw + mxo - mx) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx + mx, m->wy + my, w - (2 * c->bw),
				   mh - (2 * c->bw), 0);
			mx += WIDTH(c);
		}
		else
		{
			/* stack clients are stacked horizontally */
			w = (m->ww - tx) / (n - i);
			resize(c, m->wx + tx, m->wy, w - (2 * c->bw),
				   m->wh - (2 * c->bw), 0);
			tx += WIDTH(c);
		}
}

// -------------------------------- GridMode ----------------------------------

// https://dwm.suckless.org/patches/gridmode/
void grid(Monitor *m)
{
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
		n++;

	/* grid dimensions */
	for (rows = 0; rows <= n / 2; rows++)
		if (rows * rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = m->wh / (rows ? rows : 1);
	cw = m->ww / (cols ? cols : 1);
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
	{
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		/* adjust height/width of last row/column's windows */
		ah = ((i + 1) % rows == 0) ? m->wh - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - cw * cols : 0;
		resize(c, cx, cy, cw - 2 * c->bw + aw, ch - 2 * c->bw + ah, False);
		i++;
	}
}

void tcl(Monitor *m)
{
	int x, y, h, w, mw, sw, bdw;
	unsigned int i, n;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c;
		 c = nexttiled(c->next), n++)
		;

	if (n == 0)
		return;

	c = nexttiled(m->clients);

	mw = m->mfact * m->ww;
	sw = (m->ww - mw) / 2;
	bdw = (2 * c->bw);
	resize(c,
		   n < 3 ? m->wx : m->wx + sw,
		   m->wy,
		   n == 1 ? m->ww - bdw : mw - bdw,
		   m->wh - bdw,
		   False);

	if (--n == 0)
		return;

	w = (m->ww - mw) / ((n > 1) + 1);
	c = nexttiled(c->next);

	if (n > 1)
	{
		x = m->wx + ((n > 1) ? mw + sw : mw);
		y = m->wy;
		h = m->wh / (n / 2);

		if (h < bh)
			h = m->wh;

		for (i = 0; c && i < n / 2; c = nexttiled(c->next), i++)
		{
			resize(c,
				   x,
				   y,
				   w - bdw,
				   (i + 1 == n / 2) ? m->wy + m->wh - y - bdw : h - bdw,
				   False);

			if (h != m->wh)
				y = c->y + HEIGHT(c);
		}
	}

	x = (n + 1 / 2) == 1 ? mw : m->wx;
	y = m->wy;
	h = m->wh / ((n + 1) / 2);

	if (h < bh)
		h = m->wh;

	for (i = 0; c; c = nexttiled(c->next), i++)
	{
		resize(c,
			   x,
			   y,
			   (i + 1 == (n + 1) / 2) ? w - bdw : w - bdw,
			   (i + 1 == (n + 1) / 2) ? m->wy + m->wh - y - bdw : h - bdw,
			   False);

		if (h != m->wh)
			y = c->y + HEIGHT(c);
	}
}

// --------------------------- Full Screen ----------------------------
void togglefullscr(const Arg *arg)
{
	if (selmon->sel)
		setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}
